
import specs
import models

spec_runner = models.SpecRunner(specs = specs.all_specs)

spec_runner.run_specs()

spec_runner.print_summary()
