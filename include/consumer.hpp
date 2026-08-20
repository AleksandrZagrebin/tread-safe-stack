#pragma once

#include "ts_stack.hpp"
#include <vector>

void consume(ts_stack<int>& Q, int tasks_to_consume, int consumer_id);
