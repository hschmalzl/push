 /* ----- <push/apply_tuple.hpp> ------------------------------------------- */
#ifndef push_apply_tuple_hpp_INCLUDED
#define push_apply_tuple_hpp_INCLUDED

 /* ----- idea ------------------------------------------------------------- */
 /* the idea for this was shamelessly stolen from Johannes Schaub:
  * http://stackoverflow.com/questions/7858817/
  * unpacking-a-tuple-to-call-a-matching-function-pointer
  * 
  * The bugs are all mine.
  */
#include <tuple>

namespace push {
namespace detail {

template<int ...>
struct seq { };

template<int N, int ...S>
struct gens : gens<N-1, N-1, S...> { };

template<int ...S>
struct gens<0, S...> {
	typedef seq<S...> type;
};

template <typename Function, typename ...Args, int ...S>
void apply_helper(
	Function &&f,
	std::tuple<Args...> &args,
	seq<S...>)
{
	f(std::get<S>(args)...);
}

}

template <typename Function, typename ...Args>
void apply(Function &&f, std::tuple<Args...> &args)
{
	detail::apply_helper(
	    std::forward<Function>(f),
	    args,
	    typename detail::gens<sizeof...(Args)>::type());
}

}

#endif