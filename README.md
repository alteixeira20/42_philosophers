<p align="center">
  <img src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white" alt="C badge">
  <img src="https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white" alt="Python badge">
  <img src="https://img.shields.io/badge/Make-427819?style=for-the-badge&logo=cmake&logoColor=white" alt="Make badge">
  <img src="https://img.shields.io/badge/Shell-4EAA25?style=for-the-badge&logo=gnu-bash&logoColor=white" alt="Shell badge">
  <img src="https://img.shields.io/badge/Valgrind-773344?style=for-the-badge&logo=gnu&logoColor=white" alt="Valgrind badge">
  <img src="https://img.shields.io/badge/GDB-800000?style=for-the-badge&logo=gnu&logoColor=white" alt="GDB badge">
</p>

<h1 align="center">Philosophers</h1>
<p align="center">Thread-safe dining simulation for 42 Porto's concurrency checkpoint.</p>

<p align="center">
  <img src="docs/images/running_philo.gif" width="85%" alt="Philosophers simulation preview">
  <br>
  <em><sub>Running program with 5 philosopher, with 800ms of time to die, 200ms time to eat, 200ms time to sleep, ending when all have eaten 7 times.</sub></em>
</p>

## Table of Contents
1. [At a Glance](#at-a-glance)
2. [About](#about)
3. [Subject Compliance](#subject-compliance)
4. [Custom Tester](#custom-tester)
5. [Repository Layout](#repository-layout)
6. [Build & Integration](#build--integration)
7. [Usage Guidelines](#usage-guidelines)
8. [Program Parameters](#program-parameters)
9. [Feature Deep Dive](#feature-deep-dive)
10. [Input & Timing Mechanics](#input--timing-mechanics)
11. [Rendering & HUD](#rendering--hud)
12. [Internal Architecture](#internal-architecture)
13. [Tester Workflow](#tester-workflow)
14. [Results & Reporting](#results--reporting)

## At a Glance
> **Highlights:** Deterministic multithreading with focused tooling for peer evaluation.
- Structured around POSIX threads, one per philosopher, plus a monitor that enforces the time-to-die budget.
- Mutex-protected forks and per-philosopher meal tracking deliver global-state-free synchronization.
- Included tester stress-tests starvation, graceful stops, and invalid inputs so evaluators can reproduce verdicts fast.

## About
> **Context:** 42 Porto mandatory project exploring low-level concurrency guarantees.
- Simulates the classic dining philosophers scenario to practice race-free resource sharing.
- Emphasizes autonomous execution: once configured, the simulation manages lifecycle and shutdown on its own.
- Built to be legible for evaluations—naming, output, and structure match school expectations without extra ceremony.

## Subject Compliance
> **Highlights:** Implements every rule specified in `docs/subject_philo.pdf`.
- Accepts exactly the mandated CLI signature, validates positive integers, and rejects out-of-range values before bootstrapping (`philo/src/parser.c`).
- Allocates mutex-protected forks, avoids global variables, and keeps timing in milliseconds using `gettimeofday` and `usleep` as required.
- Stops the simulation when a philosopher dies or when every philosopher reaches the optional meal quota, honoring the subject's termination criteria.

<details>
<summary>Compliance checklist</summary>

- Allowed functions only: `pthread_*`, `usleep`, `gettimeofday`, standard memory and I/O calls.
- Timestamped logs follow the format `timestamp philosopher_id action`, guarded by a print mutex to prevent interleaving (`philo/src/utils.c`).
- One-thread-per-philosopher model with a separate supervisor mirrors the reference approach promoted during piscine and peer reviews.
</details>

## Custom Tester
> **Highlights:** Automates mandatory edge cases so you can demonstrate coverage immediately.
- `tester/tester.py` autodetects whether `philo` or `philo_bonus` is available and can be pointed to any binary with `--bin`.
- Batches survival, starvation, and validation scenarios, repeating each test to reveal timing flukes, and flags orphan processes.
- Color-coded summary keeps the focus on actionable failures (missing deaths, unexpected errors, or incomplete meal counts).

<p align="center">
  <img src="docs/images/running_tester.gif" width="85%" alt="Automated tester preview">
  <br>
  <em><sub>Preview accelerated 10× for demonstration purposes.</sub></em>
</p>

```sh
python3 tester/tester.py --bin ./philo/philo --repeat 2
```

<details>
<summary>Scenario coverage</summary>

- Negative input rejection checks guard the parser against undefined states.
- Long-running stability tests push up to 200 philosophers to expose data races or imprecise sleeps.
- Death-detection suites assert the monitor reacts within the mandated 10 ms window.
</details>

## Repository Layout
> **Highlights:** Minimal tree so reviewers can map features to files quickly.
- `philo/src`: simulation sources (`main.c`, `routines.c`, `monitor.c`, helpers).
- `philo/inc`: shared header with struct definitions and public prototypes.
- `tester/`: standalone Python harness for local or peer validation.
- `docs/`: subject PDF plus assets (tester GIF) used in this README.

## Build & Integration
> **Highlights:** Single Makefile handles compilation, cleanup, and rebuilds.
- `philo/Makefile` compiles with `-Wall -Wextra -Werror -pthread`, caches objects under `obj/`, and avoids unnecessary relinks.
- Targets: `all` (default), `clean`, `fclean`, `re`, each echoing status so you can see what changed.
- Ready for integration into 42's grading flow—build artifacts stay in the project directory, and no extra dependencies are required.

```sh
make -C philo
./philo/philo 5 800 200 200 5
make -C philo fclean
```

## Usage Guidelines
> **Highlights:** Treat timing parameters as contracts for the scheduler.
- Always compile before running; the binary lives at `philo/philo` after `make`.
- Provide at least four integers: philosophers, time_to_die, time_to_eat, time_to_sleep, plus an optional meal quota.
- Watch the output timestamps—if timing margins are tight (e.g., `time_to_die` barely above `time_to_eat + time_to_sleep`), expect immediate deaths.

```sh
./philo/philo 4 410 200 200
./philo/philo 5 800 200 200 7
```

## Program Parameters
> **Signature:** `./philo/philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]`
- `number_of_philosophers`: total philosophers and forks taking part (>= 1).
- `time_to_die`: milliseconds a philosopher may go without eating before dying.
- `time_to_eat`: milliseconds spent eating while holding two forks.
- `time_to_sleep`: milliseconds spent sleeping before thinking again.
- `number_of_times_each_philosopher_must_eat` *(optional)*: simulation stops once every philosopher reaches this count.

## Feature Deep Dive
> **Highlights:** Concurrency helpers keep the simulation predictable under load.
- `philosopher_routine` staggers forks for odd and even IDs to reduce contention (`philo/src/routines.c`).
- `precise_sleep` loops on `usleep(500)` while checking the finish flag to stay responsive to monitor events (`philo/src/utils.c`).
- `monitor_routine` couples death detection with meal quotas so the simulation stops at the earliest consistent moment (`philo/src/monitor.c`).

<details>
<summary>Fork arbitration strategy</summary>

- Even philosophers lock the right fork first; odd philosophers take the left fork after a brief stagger, preventing symmetric deadlocks.
- Single philosopher mode handles the special case: the lone thread takes one fork, waits `time_to_die`, and exits cleanly.
</details>

## Input & Timing Mechanics
> **Highlights:** Millisecond accounting underpins every state transition.
- `current_timestamp_ms` wraps `gettimeofday` to align all logs to a single simulation start (`philo/src/utils.c`).
- `precise_sleep` guards against oversleeping by polling more frequently than the death window, aiding tight timing scenarios.
- The monitor refresh window (`usleep(100)`) balances responsiveness with CPU footprint for large philosopher counts.

<details>
<summary>Lifecycle timeline</summary>

1. Simulation start timestamp recorded in `init_simulation` before any thread launches.
2. Philosophers eat -> sleep -> think loops continue while `simulation_finished` remains false.
3. Monitor stops threads by setting `simulation_finished` the moment a death or completion occurs.
</details>

## Rendering & HUD
> **Highlights:** Output stays evaluation-ready without extra noise.
- `print_state` serializes messages with `print_mutex`, keeping lines clean for log parsers or scoring scripts.
- Messages match the subject format exactly, making diff-based testers reliable.
- Death announcements are printed under the same mutex to guarantee they appear even if other threads are mid-action.

<details>
<summary>Sample log excerpt</summary>

```
0 2 is thinking
1 1 has taken a fork
2 1 has taken a fork
2 1 is eating
205 1 is sleeping
409 4 died
```
</details>

## Internal Architecture
> **Highlights:** Two core structs model shared state and actor state explicitly.
- `t_simulation` centralizes timing, fork mutexes, philosopher array, and finish flags (`philo/inc/philosophers.h`).
- `t_philosopher` keeps per-thread metrics plus a `meal_mutex` to guard `last_meal` and `meals_eaten` updates.
- `set_simulation_finished` and `is_simulation_finished` wrap the finish mutex so threads observe a consistent shutdown signal (`philo/src/main.c`).

<details>
<summary>Thread lifecycle</summary>

- `create_philosopher_threads` spins up all philosopher routines and rolls back cleanly if a launch fails.
- A dedicated monitor thread joins last, after philosopher threads finish, ensuring resources are released in order.
</details>

## Tester Workflow
> **Highlights:** Mirrors what peers expect during defence.
- Build the binary, then call the tester from it's working directory; it will prompt for mandatory vs bonus if both are present.
- Each scenario runs `repeat` times so intermittent race conditions surface quickly.
- Failures print a focused context (death or missing meal logs) so you can iterate without scrolling through noise.

```sh
cd tester
python3 tester.py
```

## Results & Reporting
> **Highlights:** Tester reports saved without noise to easy debugging
- Simulation logs present only state transitions and the terminal death message—no debug noise unless `DEBUG_MODE` is toggled in the code.
- Tester summary separates passes from failures and shows the command-line used, supporting reproducible reports.
- When every philosopher meets the meal quota, the program exits silently after the last log, matching grading expectations.
