
#ifndef RTG_GUARD_RAW_DATA_HPP
#define RTG_GUARD_RAW_DATA_HPP

#include <rtg/tensor_view.hpp>

namespace rtg {

/**
 * @brief Provides a base class for common operations with raw buffer
 *
 * For classes that handle a raw buffer of data, this will provide common operations such as equals,
 * printing, and visitors. To use this class the derived class needs to provide a `data()` method to
 * retrieve a raw pointer to the data, and `get_shape` method that provides the shape of the data.
 *
 */
template <class Derived>
struct raw_data
{
    friend bool operator==(const Derived& x, const Derived& y)
    {
        auto&& xshape = x.get_shape();
        auto&& yshape = y.get_shape();
        bool result   = x.empty() && y.empty();
        if(not result && xshape == yshape)
        {
            auto&& xbuffer = x.data();
            auto&& ybuffer = y.data();
            // TODO: Dont use tensor view for single values
            xshape.visit_type([&](auto as) {
                auto xview = make_view(xshape, as.from(xbuffer));
                auto yview = make_view(yshape, as.from(ybuffer));
                result     = xview == yview;
            });
        }
        return result;
    }

    friend bool operator!=(const Derived& x, const Derived& y) { return !(x == y); }

    template <class Stream>
    friend Stream& operator<<(Stream& os, const Derived& d)
    {
        d.visit([&](auto x) { os << x; });
        return os;
    }

    /**
     * @brief Visits a single data element at a certain index.
     *
     * @param v A function which will be called with the type of data
     * @param n The index to read from
     */
    template <class Visitor>
    void visit_at(Visitor v, std::size_t n = 0) const
    {
        auto&& s      = static_cast<const Derived&>(*this).get_shape();
        auto&& buffer = static_cast<const Derived&>(*this).data();
        s.visit_type([&](auto as) { v(*(as.from(buffer) + s.index(n))); });
    }

    /**
     * @brief Visits the data
     *
     *  This will call the visitor function with a `tensor_view<T>` based on the shape of the data.
     *
     * @param v A function to be called with `tensor_view<T>`
     */
    template <class Visitor>
    void visit(Visitor v) const
    {
        auto&& s      = static_cast<const Derived&>(*this).get_shape();
        auto&& buffer = static_cast<const Derived&>(*this).data();
        s.visit_type([&](auto as) { v(make_view(s, as.from(buffer))); });
    }

    bool single() const
    {
        auto&& s = static_cast<const Derived&>(*this).get_shape();
        return s.elements() == 1;
    }

    /**
     * @brief Retrieves a single element of data
     *
     * @param n The index to retrieve the data from
     * @tparam T The type of data to be retrieved
     * @return The element as `T`
     */
    template <class T>
    T at(std::size_t n = 0) const
    {
        T result;
        this->visit_at([&](auto x) { result = x; }, n);
        return result;
    }
};

template <class T, class... Ts>
auto visit_all(T&& x, Ts&&... xs)
{
    auto&& s                                   = x.get_shape();
    std::initializer_list<shape::type_t> types = {xs.get_shape().type()...};
    if(!std::all_of(types.begin(), types.end(), [&](shape::type_t t) { return t == s.type(); }))
        RTG_THROW("Types must be the same");
    return [&](auto v) {
        s.visit_type([&](auto as) {
            v(make_view(s, as.from(x.data())), make_view(xs.get_shape(), as.from(xs.data()))...);
        });
    };
}

} // namespace rtg

#endif
