import sys
import specs
import models


run_specific_test = sys.argv[1]

spec_runner = models.SpecRunner(
    specs = specs.all_specs
    )

if run_specific_test:
    spec_runner.run_spec(run_specific_test)
else:
    spec_runner.run_specs()

spec_runner.print_summary()
