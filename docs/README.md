# 🌐 Live-Server_SPA Documentation

## Actors:
As shown by the Use Case Diagram we have 4 different actors for this application:
- **Dev**
- **IDE**
- **Browser**
- **FileSystem**

<p align="center">
  <img src="01-requisites/UseCaseDiagram.png" alt="Use Case Diagram" width="400"/>
</p>

---

# Requisites
- **Start and stop the live Server**
- **Serve Code to browser:** the application uses server side rendering in order to allow browser to display the html code
- **Monitor the Changes that happen in the code base**
- **Reload the browser page each time changes are spotted**

<br/>

## Activity Diagrams

The first Activity diagram analyzes the startup of the live server from the perpective of the dev.

<p align="center">
  <img src="01-requisites/ActivityDiagram-1.png" alt="Activity Diagram Startup" width="400"/>
</p>

<br/>

The second Activity diagram, instead, represents the way the Live server works once it is up and running. The activity diagram shows 3 different swimlanes that signify the different envirment that partecipate in order to allow the server to be run. While on Windows (Host OS) is used to open the browser, WSL needs to be the OS through which the entire programming process takes place, so both the IDE and the filesystems must be working with WSL. Finally, Docker runs the container that includes an Apache server, which executes the C++ server for server-side rendering.

<p align="center">
  <img src="01-requisites/ActivityDiagram-2.png" alt="Activity Diagram running" width="400"/>
</p>

<br/>

# Project Structure
The envirements that compose the application, as seen before are WSL, Windows and Docker. This diagram explains the way all the components of the application communicate with each other. Docker runs a Apache server configured as a reverse proxy. Every request made is intercepted and sent to the cpp server hosted by the same machine. Together with the cpp server we have a docker volume that serves the porpuse to let apache always have the last updated version of the code base. 
On WSL, instead, we have the scripts that allow to start and stop the application, the IDE and the filesystem with which the programmer is working. Finally, since we are using Linux as an OS the syscall inotify is used to spot any change on the code at runtime and communicate it to the backend that will execute operation descibed in the following chapters.

<p align="center">
  <img src="02-Design/ClassDiagram-1.png" alt="Class Diagram" width="400"/>
</p>

## Watcher
The watcher is the class that needs to communicate with the syscall inotify. The only dependency that this class has is towards the events_register, designed for memorizing the state of the code base (modified / not modified).

<p align="center">
  <img src="02-Design/SequenceDiagramWatcher-1.png" alt="Sequence Diagram Watcher Contructorr" width="400"/>
</p>

<br/>

This function is used for monitoring the file changes. In order to make sure that the live server records them, we will have to add a watcher to both subdirectories and files included in them. That function executes this recursively, it starts from a parent directory, that is the one passed to the main, for then opening it and attach the watcher to all its children until all of them ended up being monitored.

<p align="center">
  <img src="02-Design/SequenceDiagramWatcher-3.png" alt="Sequence Diagram Watcher add_watcher_recursive" width="400"/>
</p>

<br/>

The function watch, instead, has the goal of read the changes. Whenever a modification is done by the dev the function read (which is a blocking function) return a set of events that match the changes done to the loaded code base. Once that is done we proceed to record this by calling the function add_event().

<p align="center">
  <img src="02-Design/SequenceDiagramWatcher-2.png" alt="Sequence Diagram Watcher Watch() " width="400"/>
</p>

<br/>

## Server
