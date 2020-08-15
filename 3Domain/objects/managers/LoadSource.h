//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_LOADSOURCE_H
#define BIG3DFLUFFY_LOADSOURCE_H

#include "objects/QRObject.h"
#include <fstream>

/*
LoadData defines, which source to open (one interface, may be widened in base class: for auth-data etc.)
LoadSource represents concrete loader (from file/db....). Has basic 'read' ala-template function. if params
are needed for the request (e.g., sql-request body), visitor should be added, which puts the params
into concrete source, then calls 'data' (and source will have the params inside)
*/

class LoadData {
public:
    virtual std::string getSource() const = 0;
};

class FileLoadData: public LoadData {
public:
    explicit FileLoadData(std::string file): filename(file) {}
    virtual std::string getSource() const {return filename;}
private:
    std::string filename;
};


class FileSource;
class SourceVisitor {
public:
    virtual void visitFileSource(FileSource&) = 0;
};


class LoadSource {
public:
    virtual void reopen(sptr<LoadData> data) = 0;
    virtual void accept(sptr<SourceVisitor> visitor) = 0;

    virtual int readInt() = 0;
    virtual float readDouble() = 0;
};

class FileSource: public LoadSource {
public:
    explicit FileSource(sptr<LoadData> data);
    virtual void reopen(sptr<LoadData> data);
    virtual void accept(sptr<SourceVisitor> visitor) {visitor->visitFileSource(*this);}

    virtual int readInt() {return read<int>();}
    virtual float readDouble() {return read<float>();}

    template <typename T>
    T read();
private:
    std::ifstream file;
    std::string filename;
};

template <typename T>
T FileSource::read() {
    T x;
    file >> x;
    return x;
}

#endif //BIG3DFLUFFY_LOADSOURCE_H
