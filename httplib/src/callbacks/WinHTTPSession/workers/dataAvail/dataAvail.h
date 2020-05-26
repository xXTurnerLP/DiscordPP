#pragma once

#include "pch.h"

struct RequestData;

void dataAvail_worker(RequestData* Data, LPVOID lpvDataLength, HINTERNET hRequest);