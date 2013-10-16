/**
 * @file File.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2012
 */

#ifndef FILE_H
#define FILE_H

#include <memory>
#include <string>
#include <vector>

/**
 * Class representing file on file system.
 * Currently supports only reading.
 */
class File
{
public:
	/// Modes of opening file
	enum class OpenMode { ReadOnly };

	/**
	 * Constructs object.
	 * @param fileName path to file
	 * @param mode open mode
	 * @throws Exception when something goes wrong.
	 */
	File(const std::string& fileName, OpenMode mode);

	/**
	 * Destructor.
	 * Calls close if necessary.
	 * Must be defined in cpp file, otherwise compiler would
	 * call unique_ptr destructor with incomplete type.
	 */
	~File();

	/// Get fileName passed to this object ctor.
	std::string name() const;
	/// Closes file, releasing its resources.
	void close();
	/// Get file size
	size_t size() const;
	/// Read all available data.
	std::vector<char> readAll();
	/**
	 * Read data from file.
	 * @param buff pointer to buffer where function will store data
	 * @param size number of bytes we want to read, buff must have sufficient space available
	 * @return bytes read
	 */
	size_t readData(void* buff, size_t size);
private:
	// prevent copy and assign
	File(const File&);
	File& operator=(const File&);

	// Pimpl Idiom
	class Impl;
	std::unique_ptr<Impl> impl;
};

#endif // FILE_H

