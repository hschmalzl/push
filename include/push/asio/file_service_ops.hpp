 /* ----- <push/asio/file_service_ops.hpp> --------------------------------- */
#ifndef push_asio_file_service_ops_hpp_INCLUDED
#define push_asio_file_service_ops_hpp_INCLUDED

#include <tuple>

#include <push/apply_tuple.hpp>

namespace push {
namespace asio {
namespace detail {
namespace file_service {

template <typename ImplementationType>
struct open_op {
	typedef std::tuple<boost::system::error_code> parameter_type;
	open_op(
		ImplementationType &impl,
		const boost::filesystem::path &path,
		int flags,
		mode_t mode) :
		impl(impl),
		path(path),
		flags(flags),
		mode(mode)
	{
	}
	
	void operator()(boost::system::error_code &ec)
	{
		int fh = ::open(
			path.native().c_str(),
			flags,
			mode);
		if (fh == -1)
			ec = boost::system::error_code(errno, boost::system::system_category());
		else
			this->impl.fh = fh;
	}
	
	ImplementationType &impl;
	boost::filesystem::path path;
	int flags;
	mode_t mode;
};

template <typename ImplementationType>
struct close_op {
	typedef std::tuple<boost::system::error_code> parameter_type;
	close_op(
		ImplementationType &impl) :
		impl(impl)
	{
	}
	
	void operator()(boost::system::error_code &ec)
	{
		int fh = ::close(this->impl.fh);
		if (fh != 0)
			ec = boost::system::error_code(errno, boost::system::system_category());
		else
			this->impl.fh = -1;
	}
	ImplementationType &impl;
};

template <typename ImplementationType>
struct fdatasync_op {
	typedef std::tuple<boost::system::error_code> parameter_type;
	fdatasync_op(
		ImplementationType &impl) :
		impl(impl)
	{
	}
	
	void operator()(boost::system::error_code &ec)
	{
		int ret = ::fdatasync(this->impl.fh);
		if (ret != 0)
			ec = boost::system::error_code(errno, boost::system::system_category());
	}
	ImplementationType &impl;
};

template <typename ImplementationType, typename ConstBufferSequence>
struct write_at_op {
	typedef std::tuple<boost::system::error_code, std::size_t> parameter_type;
	write_at_op(
		ImplementationType &impl,
		std::uint64_t offset,
		ConstBufferSequence buffer) :
		impl(impl),
		offset(offset),
		buffer(buffer)
	{
	}
	void operator()(boost::system::error_code &ec, std::size_t &bytes_transferred)
	{
		std::vector<iovec> buffers;
		for (const auto &e : buffer) {
			iovec iov;
			iov.iov_base = const_cast<void *>(boost::asio::buffer_cast<const void *>(e));
			iov.iov_len = boost::asio::buffer_size(e);
			buffers.push_back(iov);
		}
		auto ret = pwritev(this->impl.fh, &buffers[0], buffers.size(), offset);
		if (ret == -1) ec = boost::system::error_code(errno, boost::system::system_category());
		bytes_transferred = ret;
	}
	ImplementationType &impl;
	std::uint64_t       offset;
	ConstBufferSequence buffer;
};

template <typename ImplementationType, typename ConstBufferSequence>
struct write_op {
	typedef std::tuple<boost::system::error_code, std::size_t> parameter_type;
	write_op(
		ImplementationType &impl,
		ConstBufferSequence buffer) :
		impl(impl),
		buffer(buffer)
	{
	}
	void operator()(boost::system::error_code &ec, std::size_t &bytes_transferred)
	{
		std::vector<iovec> buffers;
		for (const auto &e : buffer) {
			iovec iov;
			iov.iov_base = const_cast<void *>(boost::asio::buffer_cast<const void *>(e));
			iov.iov_len = boost::asio::buffer_size(e);
			buffers.push_back(iov);
		}
		auto ret = writev(this->impl.fh, &buffers[0], buffers.size());
		if (ret == -1) ec = boost::system::error_code(errno, boost::system::system_category());
		bytes_transferred = ret;
	}
	ImplementationType &impl;
	ConstBufferSequence buffer;
};

template <typename ImplementationType, typename MutableBufferSequence>
struct read_at_op {
	typedef std::tuple<boost::system::error_code, std::size_t> parameter_type;
	read_at_op(
		ImplementationType &impl,
		std::uint64_t offset,
		MutableBufferSequence buffer) :
		impl(impl),
		offset(offset),
		buffer(buffer)
	{
	}
	void operator()(boost::system::error_code &ec, std::size_t &bytes_transferred)
	{
		std::vector<iovec> buffers;
		for (const auto &e : buffer) {
			iovec iov;
			iov.iov_base = const_cast<void *>(boost::asio::buffer_cast<const void *>(e));
			iov.iov_len = boost::asio::buffer_size(e);
			buffers.push_back(iov);
		}
		auto ret = preadv(this->impl.fh, &buffers[0], buffers.size(), offset);
		if (ret == -1) ec = boost::system::error_code(errno, boost::system::system_category());
		bytes_transferred = ret;
	}
	ImplementationType &impl;
	std::uint64_t offset;
	MutableBufferSequence buffer;
};

template <typename ImplementationType, typename MutableBufferSequence>
struct read_op {
	typedef std::tuple<boost::system::error_code, std::size_t> parameter_type;
	read_op(
		ImplementationType &impl,
		MutableBufferSequence buffer) :
		impl(impl),
		buffer(buffer)
	{
	}
	void operator()(boost::system::error_code &ec, std::size_t &bytes_transferred)
	{
		std::vector<iovec> buffers;
		for (const auto &e : buffer) {
			iovec iov;
			iov.iov_base = const_cast<void *>(boost::asio::buffer_cast<const void *>(e));
			iov.iov_len = boost::asio::buffer_size(e);
			buffers.push_back(iov);
		}
		auto ret = readv(this->impl.fh, &buffers[0], buffers.size());
		if (ret == -1) ec = boost::system::error_code(errno, boost::system::system_category());
		bytes_transferred = ret;
	}
	ImplementationType &impl;
	MutableBufferSequence buffer;
};

template <typename ImplementationType>
struct seek_op {
	typedef std::tuple<boost::system::error_code> parameter_type;
	seek_op(
		ImplementationType &impl,
		std::uint64_t offset) :
		impl(impl),
		offset(offset)
	{
	}
	void operator()(boost::system::error_code &ec)
	{
		auto ret = lseek(this->impl.fh, offset, SEEK_SET);
		if (ret == -1) ec = boost::system::error_code(errno, boost::system::system_category());
	}
	ImplementationType &impl;
	std::uint64_t offset;
};


}
}
}
}

#endif
