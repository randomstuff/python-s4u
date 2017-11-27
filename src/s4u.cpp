#include <string>
#include <memory>
#include <vector>
#include <functional>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include <simgrid_config.h>
#include <xbt/log.h>
#include <xbt/string.hpp>

#include <simgrid/s4u/Engine.hpp>
#include <simgrid/s4u/Actor.hpp>
#include <simgrid/s4u/Host.hpp>

namespace py = pybind11;

#include <boost/intrusive_ptr.hpp>

XBT_LOG_NEW_DEFAULT_CATEGORY(s4u_python, "S4U python");

PYBIND11_DECLARE_HOLDER_TYPE(T, boost::intrusive_ptr<T>);

namespace {

  static std::string get_simgrid_version() {
    int major, minor, patch;
    sg_version_get(&major, &minor, &patch);
    return simgrid::xbt::string_printf("%i.%i.%i", major, minor, patch);
  }

  static std::string simgrid_version = get_simgrid_version();

}

PYBIND11_PLUGIN(s4u) {

  py::module m("s4u", "Simgrid userspace API");

  m.attr("simgrid_version") = simgrid_version;

  m.def("info", [](char* s) {
    XBT_INFO("%s", s);
  });

  m.def("execute", py::overload_cast<double>(&simgrid::s4u::this_actor::execute));

  py::class_<simgrid::s4u::Engine>(m, "Engine")
    .def(py::init([](std::vector<std::string> args) -> simgrid::s4u::Engine* {
      static char noarg[] = {'\0'};
      int argc = args.size();
      std::unique_ptr<char* []> argv(new char*[argc + 1]);
      for (int i = 0; i != argc; ++i)
        argv[i]  = args[i].empty() ? noarg : &args[i].front();
      argv[argc] = nullptr;
      // Currently this can be dangling, we should wrap this somehow.
      return new simgrid::s4u::Engine(&argc, argv.get());
    }))
    .def("load_platform", &simgrid::s4u::Engine::loadPlatform)
    .def("run", &simgrid::s4u::Engine::run);

  py::class_<simgrid::s4u::Host, std::unique_ptr<simgrid::s4u::Host, py::nodelete>>(m, "Host")
    .def("by_name", &simgrid::s4u::Host::by_name);

  simgrid::s4u::ActorPtr (*createActor)(const char*, simgrid::s4u::Host*, std::function<void()>)
    =  &simgrid::s4u::Actor::createActor;

  py::class_<simgrid::s4u::Actor, simgrid::s4u::ActorPtr>(m, "Actor")
    .def("create_actor", createActor);

  return m.ptr();
}