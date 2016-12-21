import re
import json

inputFile_ref = '/home/veelken/VHbbNtuples_8_0_x/CMSSW_8_0_19/src/tthAnalysis/HiggsToTauTau/test/selEvents_analyze_3l_1tau.txt'

inputFile_test = '/home/veelken/VHbbNtuples_8_0_x/CMSSW_8_0_19/src/tthAnalysis/HiggsToTauTau/test/selEvents_analyze_3l_1tau_preselNtuple.txt'

matcher_ref = re.compile('(?P<run>\d*):(?P<lumi>\d*):(?P<evt>\d*)')
#matcher_test = re.compile('\*(?P<garbage>[0-9 ]+)\*(?P<run>[0-9 ]+)\*(?P<lumi>[0-9 ]+)\*(?P<evt>[0-9 ]+)\*')
matcher_test = matcher_ref

events_ref = set([])
events_test = set([])

def fill(files, matcher, event_set):
    for filename in files:
        file = open(filename, 'r')
        for line in file.readlines():
            match = matcher.match(line)
            if match:
                event_set.add(tuple(
                    map(int, map(match.group, ['run', 'lumi', 'evt']))
                ))

fill([ inputFile_ref ], matcher_ref, events_ref)
fill([ inputFile_test ], matcher_test, events_test)

print "Found %i REF events" % len(events_ref)
print "Found %i TEST events" % len(events_test)

print "There are %i common events" % len(events_ref.intersection(events_test))

only_ref = events_ref - events_test
print "======= REF only events (%i) ========" % len(only_ref)
for event in only_ref:
    print ":".join(map(str, event))

only_test = events_test - events_ref
print "======= TEST only events (%i) ========" % len(only_test)
for event in only_test:
    print ":".join(map(str, event))

output_file_ref = open('events_REF.txt', 'w')
for event in events_ref:
    output_file_ref.write(":".join(map(str, event)) + "\n")

##only_file_ref = open('events_exclREF.txt', 'w')
only_file_ref = open('selEvents_analyze_3l_1tau.txt', 'w')
for event in only_ref:
    only_file_ref.write(":".join(map(str, event)) + "\n")

only_file_test = open('events_exclTEST.txt', 'w')
for event in only_test:
    only_file_test.write(":".join(map(str, event)) + "\n")

