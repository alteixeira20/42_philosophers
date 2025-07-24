# 🍝 Philosophers

> A concurrent simulation of the Dining Philosophers problem, built with POSIX threads and mutexes as part of the 42 Common Core.

---

## 📘 Problem Description

The Dining Philosophers problem models a concurrency scenario where multiple philosophers sit at a circular table with one fork between each pair. Each philosopher cycles between **thinking**, **eating**, and **sleeping**.

A philosopher must hold **two forks** (the one on their left and the one on their right) to eat. Forks are **shared resources** protected by mutexes to prevent data races.

The simulation must meet the following conditions:

- **Precise Timing**:  
  - All actions are timestamped in milliseconds since the simulation started.
  - A philosopher **dies** if they don’t begin eating within `time_to_die` milliseconds since their last meal (or since the start).
  - The death must be detected and reported **within 10 ms**.

- **Fork Access**:  
  - Forks are mutex-protected to avoid simultaneous use.
  - No philosopher can take both forks unless they are both available.

- **Threaded Design**:  
  - Each philosopher is represented as a **separate thread**.
  - No global variables are allowed.
  - Output must be **synchronized**, with no overlapping or interleaved log entries.

- **Simulation Ends When**:
  - A philosopher dies from starvation, **or**
  - All philosophers have eaten at least `number_of_times_each_philosopher_must_eat` (if provided).

- **Log Output Format** (one per line):
  ```text
  timestamp_in_ms X has taken a fork
  timestamp_in_ms X is eating
  timestamp_in_ms X is sleeping
  timestamp_in_ms X is thinking
  timestamp_in_ms X died
---

## Project Goal

Implement a multithreaded simulation where:

- Philosophers take forks, eat, sleep, and think in a loop.
- Forks (shared resources) are accessed safely using **mutexes**.
- The simulation logs all actions with precise timing and formatting.
- Deaths from starvation are detected promptly.
- The simulation ends when a philosopher dies or a goal is met.

---

## Key Concepts: Mutex & Threads</strong></summary>

### 🔒 What is a Mutex?

A **mutex** (short for *mutual exclusion*) is a lock used to ensure that only **one thread at a time** can access a specific piece of code or resource — in this case, a **fork**.

Imagine a fork as a bathroom key: before a philosopher can use it (eat), they must "lock" it; when they're done, they "unlock" it so others can use it.

### Why Threads?

Each philosopher runs in their own **thread** so that they can act independently and concurrently. But since they share forks, we need mutexes to **protect fork access** from race conditions.

### Key Functions
<details>
<summary><strong>Expand too see all functions</strong></summary>

| Category         | Function                | Description                                      |
|------------------|--------------------------|--------------------------------------------------|
| Thread creation  | `pthread_create`         | Launches a new philosopher thread                |
| Thread join      | `pthread_join`           | Waits for a thread to finish (used for cleanup)  |
| Mutex setup      | `pthread_mutex_init`     | Initializes a mutex (e.g. for each fork)         |
| Mutex locking    | `pthread_mutex_lock`     | Locks a mutex — waits if it's already locked     |
| Mutex unlocking  | `pthread_mutex_unlock`   | Unlocks a mutex — allows others to access        |
| Mutex cleanup    | `pthread_mutex_destroy`  | Destroys a mutex when it's no longer needed      |
</details>

This combination of **threads** and **mutexes** allows safe and realistic concurrent behavior, making sure the simulation runs correctly without overlapping access or crashes.

---
