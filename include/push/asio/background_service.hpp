 /* ----- <push/asio/background_service.hpp> ------------------------------- */
#ifndef push_asio_background_service_hpp_INCLUDED
#define push_asio_background_service_hpp_INCLUDED

#include <thread>
#include <boost/asio.hpp>
#include <push/apply_tuple.hpp>

 /* ----- idea ------------------------------------------------------------- */
 /* when writing 
  */

namespace push {
namespace asio {

namespace detail {
namespace background_service {

template <typename Operation, typename Handler>
struct background_op {
	template <typename O, typename H>
	background_op(
		boost::asio::io_service &io_service,
		O operation,
		H handler) :
		io_service(io_service),
		work(io_service),
		operation(operation),
		handler(handler)
	{ }
	void operator()()
	{
		struct h_t {
			typename Operation::parameter_type parameter;
			Handler handler;
			h_t(Handler handler) : handler(handler) { }
			void operator()()
			{
				apply(handler, parameter);
			}
		} h(handler);
		apply(operation, h.parameter);
		io_service.post(h);
	}
	boost::asio::io_service &io_service;
	boost::asio::io_service::work work;
	typename std::remove_reference<Operation>::type operation;
	Handler handler;
};

}
}

class background_service : public boost::asio::io_service::service {
public:
	static boost::asio::io_service::id id;

	explicit background_service(boost::asio::io_service &io_service) :
		boost::asio::io_service::service(io_service),
		io_service_work(new boost::asio::io_service::work(work_io_service))
	{
		 /* TODO: find a good way to determine the number of background
		  * 	workers.
		  * TODO: find a better way to report exceptions.
		  */
		unsigned nthread = 4;
		for (unsigned i = 0; i < nthread; ++i)
			threads.emplace_back(
				[this]()
				{
					for (;;) {
						try {
							work_io_service.run();
							break;
						} catch (std::exception &) {
							std::terminate();
						}
					}
				});
	}
	~background_service()
	{
		 /* ----- shut up clang-analyze ------------------------------------ */
		 /* Call Path : shutdown_service Call virtual functions during
		  * construction or destruction will never go to a more derived class.
		  * 
		  * shutdown_service is declared "final" (see
		  * http://en.cppreference.com/w/cpp/language/final), so there can't be
		  * any more derived shutdown_service!
		  */
		//shutdown_service();
		background_service::shutdown_service();
	}
	template <typename Operation, typename Handler>
	void do_in_background(
		Operation op,
		Handler handler)
	{
		typedef typename detail::background_service::background_op<
			Operation,
			Handler> Bop;
		work_io_service.post(
		    Bop(
			get_io_service(),
			op,
			handler));
	}

private:
	void shutdown_service() override final
	{
		io_service_work.reset();
		for (auto &t : threads)
			t.join();
		threads.clear();
	}
	

	boost::asio::io_service work_io_service;
	std::unique_ptr<boost::asio::io_service::work> io_service_work;
	std::vector<std::thread> threads;
};


}
}

#endif
