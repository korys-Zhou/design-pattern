#include <iostream>
#include <atomic>
#include <mutex>

using namespace std;

//=================================================================//
class CSingle_lazy {
public:
	static CSingle_lazy* getInstance() {
		CSingle_lazy* temp = m_ps.load(memory_order_relaxed);
		atomic_thread_fence(memory_order_acquire);
		if (!temp) {
			lock_guard<mutex> lk{ m_mutex };
			temp = m_ps.load(memory_order_relaxed);
			if (!temp) {
				temp = new CSingle_lazy();
				atomic_thread_fence(memory_order_release);
				m_ps.store(temp, memory_order_relaxed);
			}
		}
		return temp;
	}

private:
	CSingle_lazy() {
		cout << "CSingle_lazy created!\n";
	}
	CSingle_lazy(const CSingle_lazy& src) = delete;
	CSingle_lazy& operator=(const CSingle_lazy& src) = delete;
	static atomic<CSingle_lazy*> m_ps;
	static mutex m_mutex;
};

atomic<CSingle_lazy*> CSingle_lazy::m_ps{ nullptr };
//=================================================================//


//=================================================================//
class CSingle_lazy_static {
public:
	static CSingle_lazy_static& getInstance() {
		static CSingle_lazy_static instance;
		return instance;
	}

private:
	CSingle_lazy_static() {
		cout << "CSingle_lazy_static created!\n";
	}
	CSingle_lazy_static(const CSingle_lazy_static& src) = delete;
	CSingle_lazy_static& operator=(const CSingle_lazy_static& src) = delete;
};
//=================================================================//


//=================================================================//
class CSingle_hungry {
public:
	static CSingle_hungry* getInstance() {
		return m_ps;
	}

private:
	CSingle_hungry() {
		cout << "CSingle_hungry created!\n";
	}
	CSingle_hungry(const CSingle_hungry& src) = delete;
	CSingle_hungry& operator=(const CSingle_hungry& src) = delete;
	static CSingle_hungry* m_ps;
};

CSingle_hungry* CSingle_hungry::m_ps = new CSingle_hungry();
//=================================================================//

int main() {
	cout << "main function started...\n";
	CSingle_lazy* csl = CSingle_lazy::getInstance();
	CSingle_lazy_static& csls = CSingle_lazy_static::getInstance();
	CSingle_hungry* csh = CSingle_hungry::getInstance();
	cout << "main function returned...\n";

	return 0;
}