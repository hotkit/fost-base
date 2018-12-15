/**
    Copyright 2007-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_JSON_CORE_HPP
#define FOST_JSON_CORE_HPP
#pragma once


#include <fost/nullable-core.hpp>
#include <fost/array>
#include <fost/string.hpp>

#include <map>
#include <variant>


namespace fostlib {


    class jcursor;
    class json;

    using json_array = std::vector<json>;
    using json_object = std::map<string, json>;


    class FOST_CORE_DECLSPEC json {
        friend class jcursor;

      public:
        using string_p = std::shared_ptr<string>;
        using array_t = json_array;
        using array_p = std::shared_ptr<array_t>;
        using object_t = json_object;
        using object_p = std::shared_ptr<object_t>;
        using element_t = std::variant<
                std::monostate,
                bool,
                int64_t,
                double,
                f5::lstring,
                string_p,
                array_p,
                object_p>;

        // We want to make sure that the underlying size types are the same
        static_assert(
                sizeof(array_t::size_type) == sizeof(object_t::size_type),
                "The underlying size types must be the same size");

      private:
        element_t m_element;
        /// Raise a `json_error` instance
        [[noreturn]] void raise(f5::lstring msg) const;

      public:
        /// Default construct to null
        json() : m_element() {}
        explicit json(t_null) : m_element() {}
        json(std::monostate) : m_element() {}
        explicit json(bool b) : m_element(b) {}
        template<typename I>
        json(I i,
             std::enable_if_t<std::is_integral<I>::value, void *> = nullptr)
        : m_element(int64_t(i)) {}
        explicit json(double d) : m_element(d) {}
        explicit json(const char *s) : m_element(std::make_shared<string>(s)) {}
        explicit json(const wchar_t *s)
        : m_element(std::make_shared<string>(s)) {}
        explicit json(string s)
        : m_element(std::make_shared<string>(std::move(s))) {}
        explicit json(string_p s) : m_element(s) {}
        json(f5::lstring s) : m_element(s) {}
        json(f5::u8view s) : m_element(std::make_shared<string>(s)) {}
        json(const array_t &a) : m_element(std::make_shared<array_t>(a)) {}
        json(array_t &&a)
        : m_element(std::make_shared<array_t>(std::move(a))) {}
        json(const object_t &o) : m_element(std::make_shared<object_t>(o)) {}
        json(object_t &&o)
        : m_element(std::make_shared<object_t>(std::move(o))) {}
        template<typename T>
        json(const nullable<T> &t) : m_element() {
            if (t) m_element = t.value();
        }
        template<typename T>
        json(nullable<T> &&t) : m_element() {
            if (t) m_element = std::move(t.value());
        }

        bool isnull() const;
        bool isatom() const;
        bool isarray() const;
        bool isobject() const;

        array_t::size_type size() const;

        bool has_key(array_t::size_type p) const;
        bool has_key(wliteral n) const { return has_key(fostlib::string(n)); }
        bool has_key(nliteral n) const { return has_key(fostlib::string(n)); }
        bool has_key(f5::u8view) const;
        bool has_key(const jcursor &p) const;
        const json &operator[](wliteral n) const {
            return (*this)[fostlib::string(n)];
        }
        const json &operator[](nliteral n) const {
            return (*this)[fostlib::string(n)];
        }
        const json &operator[](const string &k) const;
        const json &operator[](const jcursor &p) const;
        // Check that the int promotion here is safe
        static_assert(sizeof(int) <= sizeof(array_t::size_type));
        const json &operator[](int p) const {
            return (*this)[array_t::size_type(p)];
        }
        const json &operator[](array_t::size_type p) const;

        /// Fetch a value of the specified atomic type. Note that there may be
        /// multiple types that are used for a single logical JSON type, for
        /// example strings can be `std::shared_ptr<fostlib::string>` or they
        /// can be `f5::lstring`. Only an exact match will be returned.
        template<typename T>
        nullable<T> get() const {
            const T *p = std::get_if<T>(&m_element);
            if (p)
                return *p;
            else
                return null;
        }
        /// Fetch a value or the default. The value is returned only if the type
        /// exactly matches
        template<typename T>
        T get(T t) const {
            return get<T>().value_or(std::move(t));
        }

        /// Return an object if this is a JSON object
        const json_object &object() const {
            auto o = get<object_p>();
            if (o)
                return **o;
            else
                raise("This JSON value is not an object");
        }

        /// Assignment from a nullable value follows assignment rules
        template<typename T>
        json &operator=(const nullable<T> &t) {
            if (t)
                (*this) = t.value();
            else
                m_element = std::monostate{};
            return *this;
        }
        json &operator=(t_null) {
            m_element = std::monostate{};
            return *this;
        }
        json &operator=(bool b) {
            m_element = b;
            return *this;
        }
        template<
                typename I,
                std::enable_if_t<std::is_integral<I>::value, void *> = nullptr>
        json &operator=(I i) {
            m_element = int64_t(i);
            return *this;
        }
        json &operator=(double d) {
            m_element = d;
            return *this;
        }
        json &operator=(const char *s) {
            m_element = std::make_shared<string>(s);
            return *this;
        }
        json &operator=(const wchar_t *s) {
            m_element = std::make_shared<string>(s);
            return *this;
        }
        json &operator=(const string &s) {
            m_element = std::make_shared<string>(s);
            return *this;
        }
        json &operator=(string &&s) {
            m_element = std::make_shared<string>(std::move(s));
            return *this;
        }
        json &operator=(const array_t &a) {
            m_element = std::make_shared<array_t>(a);
            return *this;
        }
        json &operator=(array_t &&a) {
            m_element = std::make_shared<array_t>(std::move(a));
            return *this;
        }
        json &operator=(object_t &&o) {
            m_element = std::make_shared<object_t>(std::move(o));
            return *this;
        }
        json &operator=(const object_t &o) {
            m_element = std::make_shared<object_t>(o);
            return *this;
        }

        /// Equality checking
        bool operator==(const json &r) const;
        bool operator==(f5::u8view) const;
        template<typename V>
        bool operator!=(V &&r) const {
            return not this->operator==(std::forward<V>(r));
        }

        class FOST_CORE_DECLSPEC const_iterator {
            friend class json;
            const_iterator(const json &parent, array_t::const_iterator i);
            const_iterator(const json &parent, object_t::const_iterator i);
            using iterator_type = std::variant<
                    std::monostate,
                    array_t::const_iterator,
                    object_t::const_iterator>;

          public:
            using difference_type = std::ptrdiff_t;
            using value_type = fostlib::json;
            using pointer = typename std::add_pointer<value_type>::type;
            using reference =
                    typename std::add_lvalue_reference<value_type>::type;
            using iterator_category = std::forward_iterator_tag;

            const_iterator();

            const json &operator*() const;
            json const *operator->() const { return &**this; }
            const_iterator &operator++();

            json key() const;

            bool operator==(const_iterator r) const;
            bool operator!=(const_iterator r) const { return !(*this == r); }

          private:
            iterator_type m_iterator;
            const json *m_parent;
        };
        const_iterator begin() const;
        const_iterator end() const;

        /**
            ## Visitation

            Allow general visitation over the JSON instance through the
            use of an overload set of lambda like expressions.
         */
        template<typename... Fs>
        struct visitor_overload : std::remove_reference_t<Fs>... {
            visitor_overload(Fs &&... fs)
            : std::remove_reference_t<Fs>{std::forward<Fs>(fs)}... {}
            using std::remove_reference_t<Fs>::operator()...;
        };

        template<typename... T>
        decltype(auto) apply_visitor(T &&... t) {
            return std::visit(
                    visitor_overload<T...>(std::forward<T>(t)...), m_element);
        }
        template<typename... T>
        decltype(auto) apply_visitor(T &&... t) const {
            return std::visit(
                    visitor_overload<T...>(std::forward<T>(t)...), m_element);
        }

        /**
            ## Parsing

            Parse a JSON string returning the content. Throws on parse
            error.
        */
        static json parse(f5::u8view b);
        /// Parse a JSON string returning the content. Returns def on
        /// parse error
        static json parse(const string &, const json &def);
        /// Overloads to handle various other types we may want to parse
        static json parse(f5::buffer<unsigned char const> b) {
            return parse(f5::u8view{reinterpret_cast<char const *>(b.data()), b.size()});
        }
        static json parse(char const *l) {
            return parse(fostlib::string(l));
        }
        static json parse(wchar_t const *l) {
            return parse(fostlib::string(l));
        }

        /// Stringify the JSON data structure into the provided string instance
        static void unparse(std::string &, const json &, bool pretty);
        /// Return a string representing the JSON data structure
        static inline string unparse(const json &j, bool pretty) {
            std::string res;
            res.reserve(2048);
            unparse(res, j, pretty);
            return res;
        }

        /// Stringify the string according to JSON rules into the provided buffer
        static void unparse(std::string &, const string &);
        /// Stringify the string according to JSON rules
        static string unparse(const string &s) {
            std::string res;
            res.reserve(s.native_length() + 20); // The 20 is totally arbitrary
            unparse(res, s);
            return res;
        }
    };


}


#endif // FOST_JSON_CORE_HPP
