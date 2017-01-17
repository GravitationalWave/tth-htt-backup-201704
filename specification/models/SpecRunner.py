class SpecRunner:

    def __init__(self, specs = []):
        self.fails = {}
        self.successes = []

        self.specs = specs

    def run_specs(self):
        for spec_name in self.specs:
            self.run_spec(spec_name)

    def run_spec(self, spec_name):
        spec = self.specs[spec_name]

        try:
            result = spec()
            self.successes.append(spec_name)
        except Exception as exception:
            self.fails[spec_name] = exception

    def print_summary(self):
        print("Successful: %s, Failed: %s" % (len(self.successes), len(self.fails)))
        print(self.fails)
