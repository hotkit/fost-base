/**
    Copyright 1998-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-cli.hpp"
#include <fost/inifile.hpp>
#include <fost/insert>
#include <fost/unicode>


using namespace fostlib;


/**
    ## fostlib::exceptions::invalid_ini_line
*/


fostlib::exceptions::invalid_ini_line::invalid_ini_line(
        const fostlib::string &section,
        const fostlib::string &read,
        const fostlib::string &processed) throw()
: exception() {
    fostlib::insert(data(), "section", section);
    fostlib::insert(data(), "ini-file", read);
    fostlib::insert(data(), "processed", processed);
}


const wchar_t *const fostlib::exceptions::invalid_ini_line::message() const
        throw() {
    return L"Invalid line in INI file";
}


/**
    ## fostlib::ini_file
*/


fostlib::ini_file::ini_file(const fostlib::string &a_fileName)
: m_fileName(a_fileName) {
    loadAll();
}


void fostlib::ini_file::loadAll() {
    string section, line,
            file(utf::load_file(
                    coerce<std::wstring>(m_fileName), fostlib::string()));

    for (fostlib::string::const_iterator pc(file.begin()); pc != file.end();
         ++pc) {
        utf32 c(*pc);
        if (c == L'\n' || c == L'\r') {
            fostlib::string read(line);
            line = line.substr(0, line.find_last_not_of(L" \t") + 1);
            if (line.find_first_not_of(L" \t") != string::npos)
                line = line.substr(line.find_first_not_of(L" \t"));
            if (line == L"") {
                // Blank line - ignore it
            } else if (line[0] == L';') {
                // Comment line - ignore it
            } else if (line[0] == L'[') {
                section = line.substr(1, line.length() - 2);
                if (section == L"" || line.find(L']') == string::npos
                    || section.find_first_of(L"[]") != string::npos)
                    throw exceptions::invalid_ini_line(L"", read, section);
            } else {
                fostlib::string::size_type pos(line.find(L'='));
                if (pos == string::npos || section == L"")
                    throw exceptions::invalid_ini_line(section, read, line);
                fostlib::string name(line.substr(0, pos)),
                        value(line.substr(pos + 1));
                json js = json::parse(value, json());
                if (!value.empty() && js.isnull())
                    m_settings.push_back(boost::shared_ptr<setting<string>>(
                            new setting<string>(
                                    m_fileName, section, name, value, false)));
                else
                    m_settings.push_back(
                            boost::shared_ptr<setting<json>>(new setting<json>(
                                    m_fileName, section, name, js, false)));
            }
            line = L"";
        } else
            line += c;
    }
}
