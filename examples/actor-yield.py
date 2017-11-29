import sys
import s4u

i = sys.argv.index("--")
e = s4u.Engine(sys.argv[0:i])

def yielder(argv):
    number_of_yields = int(argv[1])
    for a in range(number_of_yields):
        s4u.yield_()
    s4u.info("I yielded " + str(number_of_yields) + " times. Goodbye now!")

e.register_function("yielder", yielder)

e.load_platform(sys.argv[i+1])
e.load_deployment(sys.argv[i+2])
e.run()
