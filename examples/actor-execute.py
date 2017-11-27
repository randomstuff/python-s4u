import sys
import s4u

def create_actor(name, host):
    def f(code):
        return s4u.Actor.create_actor(name, host, code)
    return f

i = sys.argv.index("--")
e = s4u.Engine(sys.argv[0:i])
e.load_platform(sys.argv[i+1])

tremblay = s4u.Host.by_name("Tremblay")

@create_actor("actor", tremblay)
def actor():
    s4u.info("begin")
    s4u.execute(100)
    s4u.info("end")

e.run()
