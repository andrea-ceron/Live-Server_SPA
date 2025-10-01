# 🌐 Live-Server_SPA Documentation

## 1. Actors
As shown in the *Use Case Diagram*, the application involves 4 main actors:  

- **Dev**  
- **IDE**  
- **Browser**  
- **FileSystem**  

<p align="center">
  <img src="01-requisites/UseCaseDiagram.png" alt="Use Case Diagram" width="700"/>
</p>
<p align="center"><sub><em>Figure 1 – Use Case Diagram for Live Server SPA</em></sub></p>

---

## 2. Requirements
The system must satisfy the following requirements:  

- **Start and stop the Live Server**  
- **Serve code to the browser** using *server-side rendering* (SSR)  
- **Monitor changes in the codebase**  
- **Reload the browser page whenever changes are detected**  
- **Keep the codebase independent from the Live Server**  

---

## 3. Activity Diagrams

### 3.1 Live Server Startup
The first diagram describes the startup of the Live Server from the **Dev** perspective.  

<p align="center">
  <img src="01-requisites/ActivityDiagram-1.png" alt="Activity Diagram Startup" width="700"/>
</p>
<p align="center"><sub><em>Figure 2 – Activity Diagram Startup</em></sub></p>

---

### 3.2 Live Server Running
The second diagram shows the Live Server operation once running.  

It includes three **swimlanes**:  
- **Windows (Host OS)** – used to open the browser  
- **WSL** – hosts the IDE and filesystem  
- **Docker** – runs an Apache container (configured as a reverse proxy) forwarding requests to the C++ server for SSR  

<p align="center">
  <img src="01-requisites/ActivityDiagram-2.png" alt="Activity Diagram running" width="700"/>
</p>
<p align="center"><sub><em>Figure 3 – Activity Diagram Running</em></sub></p>

---

## 4. Project Structure
The application is composed of **WSL**, **Windows**, and **Docker**.  

- **Docker**: runs Apache as a reverse proxy, forwarding requests to the C++ server. A *docker volume* ensures Apache always serves the latest codebase version.  
- **WSL**: contains scripts to start/stop the app, the IDE, and the filesystem.  
- **inotify (Linux syscall)**: detects file changes in real time and communicates them to the backend.  

<p align="center">
  <img src="02-Design/ClassDiagram-1.png" alt="Class Diagram" width="800"/>
</p>
<p align="center"><sub><em>Figure 4 – Class Diagram</em></sub></p>

---

## 5. Watcher
The **Watcher** class communicates with the `inotify` syscall.  
It depends only on **events_register**, which keeps track of the codebase update state.  

- **Constructor**  
<p align="center">
  <img src="02-Design/SequenceDiagramWatcher-1.png" alt="Sequence Diagram Watcher Constructor" width="800"/>
</p>
<p align="center"><sub><em>Figure 5 – Sequence Diagram Watcher (Constructor)</em></sub></p>

- **Recursive watcher setup**  
The method `add_watcher_recursive()` monitors the parent directory and all its subdirectories.  
<p align="center">
  <img src="02-Design/SequenceDiagramWatcher-3.png" alt="Sequence Diagram Watcher add_watcher_recursive" width="800"/>
</p>
<p align="center"><sub><em>Figure 6 – Sequence Diagram Watcher add_watcher_recursive()</em></sub></p>

- **Monitoring changes**  
The `watch()` method uses the blocking `read()` function to capture events, which are then registered with `add_event()`.  
<p align="center">
  <img src="02-Design/SequenceDiagramWatcher-2.png" alt="Sequence Diagram Watcher Watch()" width="800"/>
</p>
<p align="center"><sub><em>Figure 7 – Sequence Diagram Watcher watch()</em></sub></p>

---

## 6. Server
The **Server** has two main purposes:  

1. **Notify changes to the browser** using **Server-Sent Events (SSE)**  
2. **Perform Server-Side Rendering (SSR)**: Apache intercepts requests and forwards them to the C++ backend  

To keep the **codebase independent from the Live Server**, an endpoint injects runtime JavaScript that initializes the SSE connection in the browser.  

<p align="center">
  <img src="02-Design/Artifact.png" alt="Server Artifact" width="800"/>
</p>
<p align="center"><sub><em>Figure 8 – Server Artifact</em></sub></p>

- **Handling client requests**  
<p align="center">
  <img src="02-Design/SequenceDiagramServer-1.png" alt="Sequence Diagram Server handleClientRequest" width="800"/>
</p>
<p align="center"><sub><em>Figure 9 – Sequence Diagram Server handleClientRequest()</em></sub></p>

- **Accepting clients**  
<p align="center">
  <img src="02-Design/SequenceDiagramServer-2.png" alt="Sequence Diagram Server acceptClients" width="800"/>
</p>
<p align="center"><sub><em>Figure 10 – Sequence Diagram Server acceptClients()</em></sub></p>

---

## 7. Gateway
The **Gateway** class implements the two main endpoints:  

- **reload_endpoint()**  
<p align="center">
  <img src="02-Design/SequenceDiagramGateway-1.png" alt="Sequence Diagram Gateway reload_endpoint" width="800"/>
</p>
<p align="center"><sub><em>Figure 11 – Sequence Diagram Gateway reload_endpoint()</em></sub></p>

- **injection_endpoint()**  
<p align="center">
  <img src="02-Design/SequenceDiagramGateway-2.png" alt="Sequence Diagram Gateway injection_endpoint" width="800"/>
</p>
<p align="center"><sub><em>Figure 12 – Sequence Diagram Gateway injection_endpoint()</em></sub></p>

---

## 8. Main
The **main** program:  
1. Retrieves the path of the *docker volume*  
2. Creates **Server** and **Watcher** instances  
3. Runs both concurrently  

- `AcceptClients()` spawns a new thread for each client request  
- The parent thread runs the **Watcher**  

<p align="center">
  <img src="02-Design/SequenceDiagramMain.png" alt="Sequence Diagram Main" width="800"/>
</p>
<p align="center"><sub><em>Figure 13 – Sequence Diagram Main</em></sub></p>

---

## 9. Concurrency
Finally, the concurrency between **Watcher**, **Gateway**, and **events_register** is represented below:  

<p align="center">
  <img src="02-Design/SequenceDiagramConcurrency.png" alt="Sequence Diagram Concurrency" width="800"/>
</p>
<p align="center"><sub><em>Figure 14 – Sequence Diagram Concurrency</em></sub></p>
