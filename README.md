# Live-Server_SPA
This Live Server is a utility developed with the aim to help Single Page Application development by isolating the client-side implementation from the server one.
As a Live Server the utility allows the code to be reloaded each time it is modified in real-time.


## 🚀 Goals
- **isolate the frontend code:** avoiding building a new utility for each SPA frontend project
- **server side rendering of the frontend code**
- **reloading the website at each code change in real time:** every time the code is changed inside the IDE the utility reloads the page in the browser


## 🚀 Funzionalità
- **frontend code injection:** when the utility start, specify the directory of the project and it will load automatically the frontend dir inside the docker
- **server-side rendering:** the server inside the utility is the one responsible for the rendering of the web app.
- **spotting IDE changes:** The utility uses inotify syscall for spotting changes in the linux directory (the utility works only on linux OS or linux subsystem)
- **notify changes to browser** changes are then caught and processed so that the server reloads the web app.

## 🚀 Tecnologies
- **Apache:** used as a proxy to redirect the requests towards the server
- **C++:** used for developing the server and to monitor the changes inside the directory loaded with the utility
- **Docker:** used to run the entire application
- **bash Script** used for starting the application and to load the frontend directory in the application

## ⚙️ Requisites
Before starting the installation process let's look what are the pre-requisites for successfully running the app.
- **use a Linux OS or WSL:** you have to use linux not only to run the utility but also to run the IDE. I will explain how to run the utility especially for windows users.
- **install docker with or without Docker Desktop**

*How to run the Utility on Windows*


