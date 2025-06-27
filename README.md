
# 🍝 Philosophers

> A concurrency simulation of the Dining Philosophers problem using threads, mutexes, processes, and semaphores, developed as part of the 42 Common Core curriculum.

---

## Overview

**Philosophers** is a project that explores fundamental concepts of concurrent programming by modeling philosophers sharing forks around a table.  
It provides practical experience with:

- Thread creation & synchronization
- Mutex protection to avoid race conditions
- Deadlock avoidance strategies
- Process management & inter-process communication
- Semaphore control for shared resources

The simulation challenges you to ensure that no philosopher starves, while respecting strict timing and output requirements.

---

## 🛠 Tools & Concepts Learned

This project required mastering several low-level system programming tools:

### 🧵 Threads
- Lightweight units of execution within the same process.
- Created via `pthread_create()`.
- Allow concurrent execution of multiple philosopher routines.

### 🔒 Mutexes
- Mutual exclusion primitives to protect shared resources (forks).
- Used to avoid data races.
- Functions used:
  - `pthread_mutex_init()`
  - `pthread_mutex_lock()`
  - `pthread_mutex_unlock()`
  - `pthread_mutex_destroy()`

### 🧨 Processes *(Bonus)*
- Independent memory spaces created via `fork()`.
- Each philosopher becomes a separate process.
- Requires careful parent-child process management via:
  - `waitpid()`
  - `kill()`
  - `exit()`

### ⚙️ Semaphores *(Bonus)*
- Counting synchronization objects to control access to multiple resources.
- Allow philosophers to collectively share forks via a semaphore counter.
- Functions used:
  - `sem_open()`
  - `sem_wait()`
  - `sem_post()`
  - `sem_close()`
  - `sem_unlink()`

---

## ✅ Key Features

- Precise millisecond timestamp logging:
  ```
  timestamp_in_ms X has taken a fork
  timestamp_in_ms X is eating
  timestamp_in_ms X is sleeping
  timestamp_in_ms X is thinking
  timestamp_in_ms X died
  ```
- No global variables allowed
- Robust handling of:
  - One philosopher (edge case)
  - Deadlock prevention
  - Timely detection of starvation
- Synchronized output to prevent overlapping logs
- No memory leaks permitted

---

### 🔧 Running the Program

**Mandatory version (threads & mutexes):**

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

**Example:**

```bash
./philo 5 800 200 200 7
```

**Arguments:**

- `number_of_philosophers`: Number of philosophers (and forks)
- `time_to_die`: Time in ms before a philosopher dies if they don't eat
- `time_to_eat`: Time in ms a philosopher spends eating
- `time_to_sleep`: Time in ms a philosopher spends sleeping
- `number_of_times_each_philosopher_must_eat`: (Optional) Ends simulation when all have eaten at least this many times

**Bonus version (processes & semaphores):**

```bash
make bonus
./philo_bonus [same arguments as above]
```

---
## ⚠️ Constraints

- Must comply with the **42 Norm** coding style
- Only the allowed standard C functions and POSIX APIs are permitted
- Bonus part is only evaluated if the mandatory part is **100% correct**

---

## 📜 License

This project is part of the **42 School** educational program and is subject to its internal evaluation criteria.

---

> ✨ Happy coding and enjoy the concurrency challenge!
