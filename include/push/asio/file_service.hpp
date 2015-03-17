 /* ----- <push/asio/file_service.hpp> ------------------------------------- */
#ifndef push_asio_file_service_hpp_INCLUDED
#define push_asio_file_service_hpp_INCLUDED

#include <atomic>

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include <push/asio/background_service.hpp>
#include <push/asio/file_service_ops.hpp>

namespace push {
namespace asio {

class file_service : public boost::asio::io_service::service {
public:
	struct implementation_type {
		int fh;
	};

	static boost::asio::io_service::id id;

	explicit file_service(boost::asio::io_service &io_service) :
		boost::asio::io_service::service(io_service)
	{
	}
	~file_service()
	{
	}
	void construct(implementation_type &impl)
	{
		impl.fh = -1;
	}
	void destroy(implementation_type &impl)
	{
		if (impl.fh != -1)
			::close(impl.fh);
	}
	void open(
		implementation_type &impl,
		const boost::filesystem::path &path,
		int flags,
		mode_t mode,
		boost::system::error_code &ec)
	{
		typedef detail::file_service::open_op<implementation_type> Op;
		Op(
		    impl,
		    path.native().c_str(),
		    flags,
		    mode)(ec);
	}
	template <typename OpenHandler>
	void async_open(
		implementation_type &impl,
		const boost::filesystem::path &path,
		int flags,
		mode_t mode,
		OpenHandler handler)
	{
		typedef detail::file_service::open_op<implementation_type> Op;
		do_in_background(
		    Op(impl, path, flags, mode),
		    handler);
	}
	void close(
		implementation_type &impl,
		boost::system::error_code &ec)
	{
		typedef detail::file_service::close_op<implementation_type> Op;
		Op{impl}(ec);
	}
	template <typename CloseHandler>
	void async_close(
		implementation_type &impl,
		CloseHandler handler)
	{
		typedef detail::file_service::close_op<implementation_type> Op;
		do_in_background(
		    Op(impl),
		    handler);
	}
	void fdatasync(
		implementation_type &impl,
		boost::system::error_code &ec)
	{
		typedef detail::file_service::fdatasync_op<implementation_type> Op;
		Op{impl}(ec);
	}
	template <typename CloseHandler>
	void async_fdatasync(
		implementation_type &impl,
		CloseHandler handler)
	{
		typedef detail::file_service::fdatasync_op<implementation_type> Op;
		do_in_background(
		    Op(impl),
		    handler);
	}
	template <typename ConstBufferSequence>
	std::size_t write_some_at(
		implementation_type &impl,
		std::uint64_t offset,
		const ConstBufferSequence &buffers,
		boost::system::error_code &ec)
	{
		typedef detail::file_service::write_at_op<
			implementation_type,
			ConstBufferSequence
			> Op;
		Op op(
			impl,
			offset,
			buffers);
		std::size_t bt;
		op(ec, bt);
		return bt;
	}
	template <typename ConstBufferSequence, typename WriteHandler>
	void async_write_some_at(
		implementation_type &impl,
		std::uint64_t offset,
		const ConstBufferSequence &buffers,
		WriteHandler handler)
	{
		typedef detail::file_service::write_at_op<
			implementation_type,
			ConstBufferSequence
			> Op;
		do_in_background(
		    Op(impl, offset, buffers),
		    handler);
	}
	template <typename ConstBufferSequence>
	std::size_t write(
		implementation_type &impl,
		const ConstBufferSequence &buffers,
		boost::system::error_code &ec)
	{
		typedef detail::file_service::write_op<
			implementation_type,
			ConstBufferSequence
			> Op;
		Op op(
			impl,
			buffers);
		std::size_t bt;
		op(ec, bt);
		return bt;
	}
	template <typename ConstBufferSequence, typename WriteHandler>
	void async_write(
		implementation_type &impl,
		const ConstBufferSequence &buffers,
		WriteHandler handler)
	{
		typedef detail::file_service::write_op<
			implementation_type,
			ConstBufferSequence
			> Op;
		do_in_background(
		    Op(impl, buffers),
		    handler);
	}
	template <typename MutableBufferSequence>
	std::size_t read_some_at(
		implementation_type &impl,
		std::uint64_t offset,
		const MutableBufferSequence &buffers,
		boost::system::error_code &ec)
	{
		typedef detail::file_service::read_at_op<
			implementation_type,
			MutableBufferSequence
			> Op;
		Op op(
			impl,
			offset,
			buffers);
		std::size_t bt;
		op(ec, bt);
		return bt;
	}
	template <typename MutableBufferSequence, typename ReadHandler>
	void async_read_some_at(
		implementation_type &impl,
		std::uint64_t offset,
		const MutableBufferSequence &buffers,
		ReadHandler handler)
	{
		typedef detail::file_service::read_at_op<
			implementation_type,
			MutableBufferSequence
			> Op;
		do_in_background(
		    Op(impl, offset, buffers),
		    handler);
	}
	template <typename MutableBufferSequence, typename ReadHandler>
	void async_read(
		implementation_type &impl,
		const MutableBufferSequence &buffers,
		ReadHandler handler)
	{
		typedef detail::file_service::read_op<
			implementation_type,
			MutableBufferSequence
			> Op;
		do_in_background(
		    Op(impl, buffers),
		    handler);
	}
	template <typename MutableBufferSequence, typename ReadHandler>
	void async_read_at(
		implementation_type &impl,
		std::uint64_t offset,
		const MutableBufferSequence &buffers,
		ReadHandler handler)
	{
		typedef detail::file_service::read_at_op<
			implementation_type,
			MutableBufferSequence
			> Op;
		do_in_background(
		    Op(impl, offset, buffers),
		    handler);
	}
	void seek(
		implementation_type &impl,
		std::uint64_t offset,
		boost::system::error_code &ec)
	{
		typedef detail::file_service::seek_op<
			implementation_type
			> Op;
		Op op(
			impl,
			offset);
		op(ec);
	}
	
private:
	template <typename Op, typename Handler>
	void do_in_background(
		Op op,
		Handler handler)
	{
		auto &bs = boost::asio::use_service<push::asio::background_service>(get_io_service());
		bs.do_in_background(op, handler);
	}

	void shutdown_service() override final
	{
	}
};


}
}

#endif
