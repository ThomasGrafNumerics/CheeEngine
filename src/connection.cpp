#include "connection.h"
#include <ctime>
#include <random>

Connection::Connection () : diff_weight (0.0) { this->weight = double (rand ()) / double (RAND_MAX); }
