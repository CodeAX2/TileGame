#pragma once

namespace tg {

	template<typename T> class Nullable {

	private:
		T member;
		bool isNull;

	public:
		Nullable(T member) {
			this->member = member;
			isNull = false;
		}

		Nullable() {
			isNull = true;
		}

	public:
		Nullable<T>& operator=(const T& n) {
			member = n;
			isNull = false;
			return *this;
		}

		bool operator==(const Nullable& n) {
			return n.member == member;
		}

		bool operator==(const T& n) {
			return n == member;
		}

		operator T() { return member; }
		bool getIsNull() { return isNull; }
		T& value() { return member; }

	};

}
