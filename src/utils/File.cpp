/**
 * @file File.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2012
 */

#include "File.h"

#include "Exception.h"

#include <fstream>

class File::Impl 
{
public:
	Impl() { }

	bool isOpened() const {
		return stream.is_open();
	}

	void open(OpenMode mode);

	std::string name;
	std::fstream stream;
};

File::File(const std::string& fileName, OpenMode mode) : impl(new File::Impl()) {
	impl->name = fileName;
	impl->open(mode);
}

File::~File() {
	close();
}

void File::close() {
	if (impl->isOpened())
		impl->stream.close();
}

void File::Impl::open(OpenMode mode) {
	if (isOpened() || name.empty())
		throw Exception("File is already opened or fileName not set.");

	std::ios_base::openmode streamMode;
	switch (mode)
	{
	case File::OpenMode::ReadOnly:
		streamMode = std::ios_base::in;
		break;
	default:
		throw Exception("Unsupported OpenMode");
	}

	stream.open(name.c_str(), streamMode);
	if (!stream)
		throw Exception("Unable to open file");
}

std::string File::name() const {
	return impl->name;
}

size_t File::size() const {
	if (!impl->isOpened())
		throw Exception("File is not opened.");

	// compute length by seeking to end and getting pointer value
	size_t len;
	impl->stream.seekg(0, std::ios::end);
	len = static_cast<size_t>(impl->stream.tellg());
	impl->stream.seekg(0, std::ios::beg);

	return len;
}

size_t File::readData(void* buff, size_t size) {
	if (!impl->isOpened())
		throw Exception("File is not opened.");

	impl->stream.read(reinterpret_cast<char*>(buff), size);
	return static_cast<size_t>(impl->stream.gcount());
}

std::vector<char> File::readAll() {
	std::vector<char> res(size());
	readData(reinterpret_cast<void*>(res.data()), res.size());
	return res;
}
