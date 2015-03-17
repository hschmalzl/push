#ifndef push_asio_file_hpp_INCLUDED
#define push_asio_file_hpp_INCLUDED

#include <push/asio/file_service.hpp>

namespace push {
namespace asio {
	
class file : public boost::asio::basic_io_object<file_service> {
public:
	explicit file(boost::asio::io_service &io_service) :
		boost::asio::basic_io_object<file_service>(io_service)
	{
	}
	void open(
		const boost::filesystem::path &path,
		int flags,
		mode_t mode,
		boost::system::error_code &ec)
	{
		return this->get_service().open(
			this->get_implementation(),
			path,
			flags,
			mode,
			ec);
	}
	void open(
		const boost::filesystem::path &path,
		int flags,
		mode_t mode)
	{
		boost::system::error_code ec;
		open(path, flags, mode, ec);
		if (ec) throw boost::system::system_error(ec);
	}
	template <typename OpenHandler>
	void async_open(
		const boost::filesystem::path &path,
		int flags,
		mode_t mode,
		OpenHandler handler)
	{
		return this->get_service().async_open(
			this->get_implementation(),
			path,
			flags,
			mode,
			handler);
	}
	void close(
		boost::system::error_code &ec)
	{
		return this->get_service().close(
			this->get_implementation(),
			ec);
	}
	void close()
	{
		boost::system::error_code ec;
		close(ec);
		if (ec) throw boost::system::system_error(ec);
	}
	template <typename OpenHandler>
	void async_close(
		OpenHandler &&handler)
	{
		return this->get_service().async_close(
			this->get_implementation(),
			handler);
	}
	void fdatasync(
		boost::system::error_code &ec)
	{
		return this->get_service().fdatasync(
			this->get_implementation(),
			ec);
	}
	void fdatasync()
	{
		boost::system::error_code ec;
		fdatasync(ec);
		if (ec) throw boost::system::system_error(ec);
	}
	template <typename OpenHandler>
	void async_fdatasync(
		OpenHandler &&handler)
	{
		return this->get_service().async_fdatasync(
			this->get_implementation(),
			handler);
	}
	template <typename ConstBufferSequence>
	void write_some_at(
		std::uint64_t offset,
		const ConstBufferSequence &buffers,
		boost::system::error_code &ec)
	{
		return this->get_service().write_some_at(
			this->get_implementation(),
			offset,
			buffers,
			ec);
	}
	template <typename ConstBufferSequence>
	void write_some_at(
		std::uint64_t offset,
		const ConstBufferSequence &buffers)
	{
		boost::system::error_code ec;
		return this->get_service().write_some_at(
			this->get_implementation(),
			offset,
			buffers,
			ec);
		if (ec) throw boost::system::system_error(ec);
	}
	template <typename ConstBufferSequence, typename WriteHandler>
	void async_write_some_at(
		std::uint64_t offset,
		const ConstBufferSequence &buffers,
		WriteHandler handler)
	{
		return this->get_service().async_write_some_at(
			this->get_implementation(),
			offset,
			buffers,
			handler);
	}
	template <typename ConstBufferSequence, typename WriteHandler>
	void async_write(
		const ConstBufferSequence &buffers,
		WriteHandler handler)
	{
		return this->get_service().async_write(
			this->get_implementation(),
			buffers,
			handler);
	}
	template <typename MutableBufferSequence>
	void read_some_at(
		std::uint64_t offset,
		const MutableBufferSequence &buffers,
		boost::system::error_code &ec)
	{
		return this->get_service().read_some_at(
			this->get_implementation(),
			offset,
			buffers,
			ec);
	}
	template <typename MutableBufferSequence>
	void read_some_at(
		std::uint64_t offset,
		const MutableBufferSequence &buffers)
	{
		boost::system::error_code ec;
		return this->get_service().read_some_at(
			this->get_implementation(),
			offset,
			buffers,
			ec);
		if (ec) throw boost::system::system_error(ec);
	}
	template <typename MutableBufferSequence, typename ReadHandler>
	void async_read_some_at(
		std::uint64_t offset,
		const MutableBufferSequence &buffers,
		ReadHandler handler)
	{
		return this->get_service().async_read_some_at(
			this->get_implementation(),
			offset,
			buffers,
			handler);
	}
	template <typename ConstBufferSequence, typename ReadHandler>
	void async_read(
		const ConstBufferSequence &buffers,
		ReadHandler handler)
	{
		return this->get_service().async_read(
			this->get_implementation(),
			buffers,
			handler);
	}
	template <typename MutableBufferSequence, typename ReadHandler>
	void async_read_at(
		std::uint64_t offset,
		const MutableBufferSequence &buffers,
		ReadHandler handler)
	{
		return this->get_service().async_read_at(
			this->get_implementation(),
			offset,
			buffers,
			handler);
	}
	void seek(
		std::uint64_t offset,
		boost::system::error_code &ec)
	{
		return this->get_service().seek(
			this->get_implementation(),
			offset,
			ec);
	}
	void seek(
		std::uint64_t offset)
	{
		boost::system::error_code ec;
		seek(offset, ec);
		if (ec) throw boost::system::system_error(ec);
	}

};

}
}

#endif