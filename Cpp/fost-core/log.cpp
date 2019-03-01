/**
    Copyright 2010-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"
#include "log.hpp"
#include <fost/insert.hpp>

#include <utility>


using namespace fostlib;


const module fostlib::log::detail::c_legacy("--unknown");


/**
    ## fostlib::logging::message
*/


fostlib::log::message::message(std::size_t l, nliteral n, const json &j)
: when(timestamp::now()),
  level(l),
  name(n),
  body(j),
  m_module(log::detail::c_legacy) {}


fostlib::log::message::message(
        const fostlib::module &m, std::size_t l, nliteral n, const json &j)
: when(timestamp::now()), level(l), name(n), body(j), m_module(m) {}


fostlib::log::message::message(const fostlib::module &m, const json &j)
: opt_module(fostlib::module(
          m,
          [&]() {
              auto mod = j["module"];
              if (mod.isarray()) {
                  std::string modpath;
                  for (const auto &p : mod) {
                      if (not modpath.empty()) modpath += '/';
                      modpath +=
                              static_cast<std::string_view>(coerce<string>(p));
                  }
                  return modpath;
              } else {
                  return static_cast<std::string>(coerce<string>(j["module"]));
              }
          }())),
  opt_name(coerce<string>(j["level"]["name"])),
  when(coerce<timestamp>(j["when"])),
  level(coerce<std::size_t>(j["level"]["value"])),
  name(opt_name.value().c_str()),
  body(j["body"]),
  m_module(opt_module.value()) {}


fostlib::log::message::message(const message &m)
: opt_module(m.opt_module),
  opt_name(m.opt_name),
  when(m.when),
  level(m.level),
  name(opt_name ? opt_name.value().c_str() : m.name()),
  body(m.body),
  m_module(opt_module ? opt_module.value() : m.m_module) {}


fostlib::log::message::message(message &&m)
: opt_module(std::move(m.opt_module)),
  opt_name(std::move(m.opt_name)),
  when(std::move(m.when)),
  level(m.level()),
  name(opt_name ? opt_name.value().c_str() : m.name()),
  body(std::move(m.body)),
  m_module(opt_module ? opt_module.value() : m.m_module) {}


json fostlib::coercer<json, fostlib::log::message>::coerce(
        const fostlib::log::message &m) {
    json::object_t js, lv;

    lv["value"] = fostlib::coerce<json>(m.level());
    lv["name"] = m.name();

    js["when"] = fostlib::coerce<json>(m.when());
    js["module"] = fostlib::coerce<json>(m.module());
    js["level"] = std::move(lv);
    js["body"] = m.body();

    return js;
}


/**
    ## fostlib::log functions
*/


void fostlib::log::log(fostlib::log::message m) {
    fostlib::log::detail::log_proxy::proxy().log(std::move(m));
}


void fostlib::log::flush() {
    fostlib::log::detail::log_proxy::proxy().exec([]() {});
}


/**
    ## fostlib::log::log_object
*/


fostlib::log::detail::log_object::log_object(
        const module &m, std::size_t level, fostlib::nliteral name)
: part(m), level(level), name(name) {}


fostlib::log::detail::log_object::log_object(
        std::size_t level, fostlib::nliteral name)
: part(log::detail::c_legacy), level(level), name(name) {}


fostlib::log::detail::log_object::log_object(log_object &&right)
: part(right.part),
  level(right.level),
  name(std::move(right.name)),
  log_message(std::move(right.log_message)) {}


fostlib::log::detail::log_object::~log_object() try {
    fostlib::log::log(part, level, name, log_message);
} catch (...) { absorb_exception(); }
