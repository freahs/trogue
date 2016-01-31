/**
 * Copyright 2016 Fredrik Åhs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LIBTYRA_ASSERT_H
#define LIBTYRA_ASSERT_H

#ifdef NDEBUG

#include <stdexcept>
#include <string>
#define ASSERT_ERROR(condition, message) if(!(condition)) throw std::runtime_error(std::string(message));
#define ASSERT_WARNING(...);

#else

#include <cassert>
#define ASSERT_ERROR(condition, message) if(!(condition)) assert(condition && message);
#define ASSERT_WARNING(condition, message) if(!(condition)) assert(condition && message);

#endif

#endif
