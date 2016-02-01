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

#ifndef TYRA_MANAGER_H
#define TYRA_MANAGER_H

namespace tyra {

	class World;

	class Manager {
	private:
		World* m_world;

	protected:
		World& world() { return *m_world; }

	public:
		Manager() : m_world(nullptr) { }
		void world(World& world) { m_world = &world; }
	};
}

#endif