/**
    Copyright 1999-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core.hpp"
#include <fost/detail/utility.hpp>
#include <fost/detail/coerce.hpp>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


using namespace fostlib;


namespace {


    const char *c_whitespace_utf8 = " \n\r\t";

    const std::u32string_view whitespace_code_point =
            U"\t\n\r\f\u0020\u00A0\u1680\u180E\u2000\u2001\u2002\u2003\u2004"
            U"\u2005\u2006\u2007\u2008\u2009\u200A\u200B\u202F\u205F\u3000"
            U"\uFEFF";


    template<typename C>
    auto in_seq_pred(C seq) {
        return [seq](char32_t ch) {
            return std::find(seq.begin(), seq.end(), ch) != seq.end();
        };
    }

    template<typename S, typename C>
    S ltrim(S const &text, C seq) {
        auto pos = text.begin(), end = text.end();
        auto text_start = std::find_if_not(pos, end, in_seq_pred(seq));
        return S{text_start, end};
    }

    template<typename S, typename C>
    S rtrim(S const &text, C seq) {
        auto pos = text.begin(), end = text.end(), trim_start = end;
        auto pred = in_seq_pred(seq);
        while (pos != end) {
            if (pred(*pos)) {
                if (trim_start == end) trim_start = pos;
            } else {
                trim_start = end;
            }
            ++pos;
        }
        return S{text.begin(), trim_start};
    }

    template<typename S, typename C>
    S uutrim(const S &text, C seq) {
        return rtrim(ltrim(text, seq), seq);
    }
    template<typename S, typename C>
    nullable<S> utrim(const S &text, C seq) {
        S t = uutrim(text, seq);
        if (t.empty())
            return null;
        else
            return t;
    }

    template<typename S>
    std::pair<S, nullable<S>> ipartition(const S &text, f5::u8view u8bound) {
        // TODO We'll want to get rid of this allocation
        std::string bound{u8bound};
        S first;
        nullable<S> second;

        typename S::size_type start = text.underlying().find(bound);
        if (start == std::string::npos)
            first = trim(text).value_or(S());
        else {
            first = trim(S(text.underlying().substr(0, start))).value_or(S());
            second = trim(S(text.underlying().substr(
                    start + bound.length(), std::string::npos)));
        }

        return std::make_pair(first, second);
    }


}


string fostlib::guid() {
    boost::uuids::uuid guid = boost::uuids::random_generator()();
    std::stringstream ss;
    ss << guid;
    return string(ss.str());
}


nullable<f5::u8view> fostlib::trim(f5::u8view text) {
    return ::utrim(text, whitespace_code_point);
}
nullable<f5::u8view> fostlib::trim(f5::u8view text, f5::u8view chars) {
    return ::utrim(text, chars);
}
nullable<f5::u8view> fostlib::trim(nullable<f5::u8view> text) {
    if (not text)
        return null;
    else
        return trim(text.value());
}


fostlib::utf8_string fostlib::replace_all(
        const fostlib::utf8_string &text,
        f5::u8view findThis_u8,
        const fostlib::nullable<f5::u8view> &replaceWith) {
    // TODO Get rid of this allocation. It's needed to support the find
    // calls. Probably this can be replaced with a std::string_view in C++17
    std::string findThis{findThis_u8};
    fostlib::utf8_string::impl_type ret = text.underlying();
    if (not replaceWith) {
        for (fostlib::utf8_string::impl_type::size_type p(ret.find(findThis));
             p != fostlib::utf8_string::impl_type::npos;
             p = ret.find(findThis, p)) {
            ret = ret.substr(0, p) + ret.substr(p + findThis.length());
        }
    } else {
        for (fostlib::utf8_string::impl_type::size_type p(ret.find(findThis));
             p != fostlib::utf8_string::impl_type::npos;
             p = ret.find(findThis, p + replaceWith.value().bytes())) {
            ret = ret.substr(0, p) + replaceWith.value()
                    + ret.substr(p + findThis.length());
        }
    }
    return ret;
}


fostlib::string fostlib::replace_all(
        const fostlib::string &text,
        f5::u8view findThis,
        const fostlib::nullable<f5::u8view> &replaceWith) {
    fostlib::string ret = text;
    if (not replaceWith) {
        for (fostlib::string::size_type p(ret.find(findThis));
             p != string::npos; p = ret.find(findThis, p)) {
            ret = ret.substr(0, p) + ret.substr(p + findThis.bytes());
        }
    } else {
        for (fostlib::string::size_type p(ret.find(findThis));
             p != string::npos;
             p = ret.find(findThis, p + replaceWith.value().bytes())) {
            ret = ret.substr(0, p) + replaceWith.value()
                    + ret.substr(p + findThis.bytes());
        }
    }
    return ret;
}
fostlib::nullable<fostlib::string> fostlib::replace_all(
        const fostlib::nullable<f5::u8view> &text,
        f5::u8view findThis,
        const fostlib::nullable<f5::u8view> &replaceWith) {
    if (not text)
        return null;
    else
        return replace_all(string(text.value()), findThis, replaceWith);
}


std::pair<fostlib::utf8_string, nullable<fostlib::utf8_string>>
        fostlib::partition(const fostlib::utf8_string &text, f5::u8view bound) {
    return ipartition(text, bound);
}


std::pair<fostlib::utf8_string, nullable<fostlib::utf8_string>>
        fostlib::partition(
                const nullable<fostlib::utf8_string> &text, f5::u8view bound) {
    if (not text)
        return std::make_pair(fostlib::utf8_string(), null);
    else
        return partition(text.value(), bound);
}


std::pair<fostlib::utf8_string, nullable<fostlib::utf8_string>>
        fostlib::partition(const fostlib::utf8_string &text) {
    fostlib::utf8_string first;
    nullable<fostlib::utf8_string> second;

    fostlib::utf8_string::impl_type::size_type start =
            text.underlying().find_first_of(c_whitespace_utf8);
    if (start == std::string::npos)
        first = trim(text).value_or(utf8_string());
    else {
        first = trim(fostlib::utf8_string(text.underlying().substr(0, start)))
                        .value_or(utf8_string());
        second =
                trim(fostlib::utf8_string(text.underlying().substr(start + 1)));
        if (second.value().empty()) second = null;
    }

    return std::make_pair(first, second);
}


std::pair<fostlib::utf8_string, nullable<fostlib::utf8_string>>
        fostlib::partition(const nullable<fostlib::utf8_string> &text) {
    if (not text)
        return std::make_pair(fostlib::utf8_string(), null);
    else
        return partition(text.value());
}


std::pair<fostlib::string, nullable<fostlib::string>>
        fostlib::partition(const fostlib::string &text, f5::u8view bound) {
    fostlib::string first;
    nullable<fostlib::string> second;

    fostlib::string::size_type start = text.find(bound);
    if (start == fostlib::string::npos)
        first = trim(text).value_or(fostlib::string());
    else {
        first = trim(text.substr(0, start)).value_or(fostlib::string());
        second =
                trim(text.substr(start + bound.bytes(), fostlib::string::npos));
    }

    return std::make_pair(first, second);
}


std::pair<fostlib::string, nullable<fostlib::string>> fostlib::partition(
        const nullable<fostlib::string> &text, f5::u8view bound) {
    if (not text)
        return std::make_pair(fostlib::string(), null);
    else
        return partition(text.value(), bound);
}


std::pair<fostlib::string, nullable<fostlib::string>>
        fostlib::partition(const fostlib::string &text) {
    fostlib::string first;
    nullable<fostlib::string> second;

    fostlib::string::size_type start = text.find_first_of(c_whitespace_utf8);
    if (start == fostlib::string::npos) {
        first = trim(text).value_or(string());
    } else {
        first = trim(text.substr(0, start)).value_or(string());
        second = trim(text.substr(start + 1, fostlib::string::npos));
    }

    return std::make_pair(first, second);
}


std::pair<fostlib::string, nullable<fostlib::string>>
        fostlib::partition(const nullable<fostlib::string> &text) {
    if (not text)
        return std::make_pair(fostlib::string(), null);
    else
        return partition(text.value());
}


split_type
        fostlib::split(const fostlib::string &text, const fostlib::string &on) {
    split_type ret;
    std::pair<fostlib::string, nullable<fostlib::string>> parts(
            partition(text, on));
    while (parts.second) {
        if (!parts.first.empty()) ret.push_back(parts.first);
        parts = partition(parts.second.value(), on);
    }
    if (!parts.first.empty()) ret.push_back(parts.first);
    return ret;
}


nullable<string> fostlib::concat(
        const nullable<f5::u8view> &left,
        f5::u8view mid,
        const nullable<f5::u8view> &right) {
    if (not left)
        return right;
    else if (not right)
        return left;
    else
        return f5::u8view{left.value() + mid} + right.value();
}
nullable<string> fostlib::concat(
        const nullable<f5::u8view> &left, const nullable<f5::u8view> &right) {
    if (not left && not right)
        return null;
    else if (not left && right)
        return right;
    else if (left && not right)
        return left;
    else
        return left.value() + right.value();
}


std::pair<string, nullable<string>>
        fostlib::crack(f5::u8view text, f5::u8view open, f5::u8view close) {
    string nut = trim(string(text)).value_or(string());
    string::size_type spos = nut.find(open);
    if (spos == string::npos)
        return std::make_pair(nut, null);
    else {
        string::size_type epos = nut.find(close, spos + open.bytes());
        if (epos == string::npos)
            return std::make_pair(nut, null);
        else {
            if (open != close) {
                string::size_type mpos = nut.find(open, spos + open.bytes());
                while (mpos <= epos) {
                    epos = nut.find(close, epos + close.bytes());
                    mpos = nut.find(open, mpos + open.bytes());
                }
            }
            return std::make_pair(
                    trim(nut.substr(0, spos)).value_or(string()),
                    trim(nut.substr(
                            spos + open.bytes(), epos - spos - open.bytes())));
        }
    }
}
