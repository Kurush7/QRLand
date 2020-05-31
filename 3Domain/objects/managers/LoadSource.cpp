//
// Created by kurush on 31.05.2020.
//

#include "LoadSource.h"


FileSource::FileSource(shared_ptr<LoadData> data) {
    filename = data->getSource();
    file = ifstream(filename);
    if (!file.is_open())
        throw QRBadSourceException(__FILE__, __LINE__, __TIME__, "file not opened");
}

void FileSource::reopen(shared_ptr<LoadData> data) {
    filename = data->getSource();
    file = ifstream(filename);
    if (!file.is_open())
        throw QRBadSourceException(__FILE__, __LINE__, __TIME__, "file not opened");
}