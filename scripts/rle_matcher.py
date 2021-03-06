#!/usr/bin/env python

import logging, argparse, os, sys, re, ROOT, array, subprocess
from tthAnalysis.HiggsToTauTau.tthAnalyzeSamples_2016 import samples_2016 as samples

def is_dict_full(d):
  '''Checks whether all values in a given dictionary are present
  Args:
    d: dictionary, which is checked here

  Returns: True, if none of its values are empty strings,
           False otherwise
  '''
  return len(filter(lambda x: x == '', d.values())) == 0

if __name__ == '__main__':
  logging.basicConfig(
    stream = sys.stdout,
    level  = logging.INFO,
    format = '%(asctime)s - %(levelname)s: %(message)s'
  )

  class SmartFormatter(argparse.HelpFormatter):
    def _split_lines(self, text, width):
      if text.startswith('R|'):
        return text[2:].splitlines()
      return argparse.HelpFormatter._split_lines(self, text, width)

  parser = argparse.ArgumentParser(formatter_class = lambda prog: SmartFormatter(prog, max_help_position = 35))
  parser.add_argument('-s', '--sample-name', metavar = 'name', required = True, type = str,
                      help = 'R|Sample name')
  parser.add_argument('-f', '--file', metavar = 'file', required = True, type = str,
                      help = 'R|Path to file containing a list of RLE numbers')
  parser.add_argument('-o', '--output', metavar = 'file', required = False, type = str, default = '',
                      help = 'R|File where the list of matched ROOT files is saved')
  parser.add_argument('-d', '--directory', metavar = 'directory', required = False, type = str, default = '',
                      help = 'R|Directory where all RLE numbers are stored')
  parser.add_argument('-v', '--verbose', dest = 'verbose', action = 'store_true', default = False,
                      help='R|Enable verbose printout')
  args = parser.parse_args()

  if args.verbose:
    logging.getLogger().setLevel(logging.DEBUG)

  rle_file = args.file
  sample_name = args.sample_name
  output_file = args.output
  grep_directory = args.directory

  if not os.path.isfile(rle_file):
    logging.error("No such file: '{rle_filename}'".format(
      rle_filename = rle_file,
    ))
    sys.exit(1)

  if output_file and not os.path.isdir(os.path.dirname(output_file)):
    logging.error("Parent directory of '{output_file}' doesn't exist".format(
      output_file = output_file,
    ))
    sys.exit(1)

  if grep_directory and not os.path.isdir(grep_directory):
    logging.error("Grep directory '{grep_directory}' doesn't exist".format(
      grep_directory = grep_directory,
    ))
    sys.exit(1)

  sample_key = ''
  for s_key, s_value in samples.iteritems():
    if s_value['process_name_specific'] == sample_name: #TODO should we do case-insensitive comparison instead?
      sample_key = s_key
      break

  if not sample_key:
    logging.error("Invalid sample name: '{sample_name}'".format(sample_name = sample_name))
    sys.exit(1)

  # read the RLE numbers and form a dictionary { RLE number : root file it contains it }
  rle_pattern = re.compile('\d+:\d+:\d+')
  rles = {}
  with open(rle_file, 'r') as f:
    for line in f:
      line = line.rstrip('\n')
      if not line: continue

      rle_match = rle_pattern.match(line)
      if not rle_match:
        logging.error("Line '{unmatched_line}' doesn't look like a RLE number".format(
          unmatched_line = line,
        ))
        sys.exit(1)

      rles[line] = ''

  # since 2016 Ntuples have no "secondary paths" we can read off the location to the Ntuples trivially
  sample_path = samples[sample_key]['local_paths'][0]['path']

  if grep_directory:
    # here we try to grep RLE numbers instead of looping over ROOT files
    rle_matched_files = []

    # let's create a grep command first
    grep_rle_list = '\\|'.join(rles.keys())
    grep_subdir = os.path.join(grep_directory, sample_name)
    if not os.path.isdir(grep_subdir):
      logging.error("No such directory: '{grep_subdir}'".format(grep_subdir = grep_subdir))
      sys.exit(1)
    grep_cmd_str = "grep -nr '%s' %s | awk -F ':' '{print $1}' | uniq" % \
                   (grep_rle_list, os.path.join(grep_subdir, '*'))
    grep_process = subprocess.Popen(
      grep_cmd_str,
      stdout = subprocess.PIPE,
      stderr = subprocess.PIPE,
      shell  = True
    )
    grep_stdout, grep_stderr = grep_process.communicate()
    if grep_stderr:
      logging.error("There was an error:\n%s" % grep_stderr)
      sys.exit(1)

    grep_result = map(os.path.basename, filter(lambda x: x != '', grep_stdout.split()))
    rle_matched_files = map(lambda x: os.path.join(
      sample_path, '000%d' % (int(x[:x.rfind('.')]) / 1000), 'tree_{i}.root'.format(i = x[:x.rfind('.')])), grep_result)
  else:
    # instead of forming a list of files let's loop over the subfolders and the files therein instead
    for subdir_basename in os.listdir(sample_path):
      subdir = os.path.join(sample_path, subdir_basename)
      for rootfile_basename in os.listdir(subdir):
        rootfile = os.path.join(subdir, rootfile_basename)
        logging.debug("Processing file '{rootfile}'".format(
          rootfile = rootfile,
        ))

        # open the file
        ch_root = ROOT.TChain("tree")
        ch_root.AddFile(rootfile)

        run_a  = array.array('I', [0])
        lumi_a = array.array('I', [0])
        evt_a  = array.array('L', [0])

        ch_root.SetBranchAddress("run",  run_a)
        ch_root.SetBranchAddress("lumi", lumi_a)
        ch_root.SetBranchAddress("evt",  evt_a)

        nof_entries = ch_root.GetEntries()
        for i in range(nof_entries):
          ch_root.GetEntry(i)
          rle_i = ':'.join(map(str, [run_a[0], lumi_a[0], evt_a[0]]))
          if rle_i in rles:
            if rles[rle_i]:
              logging.error("Something's wrong: files {first_file} and {second_file} contain the same RLE number".format(
                first_file = rles[rle_i],
                second_file = rootfile,
              ))
              sys.exit(1)
            rles[rle_i] = rootfile
            logging.debug("Got a match '{rle_number}'".format(
              rle_number = rle_i
            ))
            if is_dict_full(rles): break
        if is_dict_full(rles): break
      if is_dict_full(rles): break

    if not is_dict_full(rles):
      missing_rles = []
      for rle_i in rles:
        if not rles[rle_i]:
          missing_rles.append(rle_i)
      logging.error("There are still some RLE numbers left for which no ROOT file was found: {rle_list}".format(
        rle_list = ", ".join(missing_rles),
      ))

    # let's make the list of matched ROOT files unique
    rle_matched_files = list(set(rles.values()))


  logging.debug("Got matches to all RLE numbers")
  for rle_match in rle_matched_files:
    print("{rle_match}".format(
      rle_match = rle_match,
    ))

  # optionally, let's save the file
  if output_file:
    logging.debug("Writing to file '{output_file}'".format(
      output_file = output_file,
    ))
    with open(output_file, 'w') as f:
      for rle_match in rle_matched_files:
        f.write("%s\n" % rle_match)

  logging.debug("Done!")