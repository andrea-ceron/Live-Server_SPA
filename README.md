
# 🌐 Live-Server_SPA

**Live-Server_SPA** is a utility designed to simplify the development of **Single Page Applications (SPA)** by isolating the client-side implementation from the server-side one.  
As a *live server*, it provides **real-time reload** whenever the code is modified.

<br><br>
---

<br><br>



## 🎯 Goals

- 🔹 **Isolate frontend code** → avoid building a new server for every SPA project  
- 🔹 **Server-side rendering** of the frontend code  
- 🔹 **Real-time reload** → every change in the code is instantly reflected in the browser  

<br><br>
---
<br><br>




## ⚡ Features

- 🗂️ **Frontend code injection** → when the utility starts, specify the project directory and the frontend is automatically loaded into the Docker container  
- 🎭 **Server-side rendering** → the internal server handles web app rendering  
- 👀 **File monitoring with inotify** → detects changes in the Linux filesystem  
- 🔄 **Browser reload notifications** → modifications are processed and the web app is reloaded  

<br><br>
---
<br><br>




## 🛠️ Technologies

- 🖧 **Apache** → proxy for request redirection  
- 💻 **C++** → used to build the server and monitor filesystem changes  
- 📦 **Docker** → runs the entire application  
- 🐚 **Bash scripts** → used for application startup and frontend directory loading  

<br><br>
---
<br><br>




## 📋 Requirements

Before running the app, make sure you have:

- 🐧 **Linux OS or WSL**  
  > The utility relies on the Linux `inotify` syscall, so both the utility **and your IDE** must run on Linux (or WSL on Windows).  
- 🐳 **Docker (with or without Docker Desktop)**  

<br><br>
---
<br><br>




## 💻 Requirements for Running on Windows

1. Install **WSL** (e.g., Ubuntu).  
2. **Copy your project folder inside WSL**.  
3. Open **VS Code with the WSL extension**:  
   - Install **Remote - WSL** extension  
   - Click the **colored rectangle in the bottom-left corner**   

   ![colored rectangle in the bottom-left corner](docs/image_1.png)

   *Connect to WSL* 


   ![Connect to WSL](docs/image_2.png)

   *VS Code running on WSL* 


   ![VS Code running on WSL](docs/image_3.png)

4. Once connected, you are fully working inside your Linux subsystem.  
   > Every code change will now be detected in real-time and reloaded by the application 🚀  

4. In order to start the Utility smoothly I decided to implement inside the bash script startup.sh a simple script that allows to find the project autonomuosly. All that you have to do is type after the command the name of the directory that includes the client dir that you want to run:
```bash
sh startup.sh <project-name>
```
Since we are not specifying the relative path to the project the script tries to find the project. In order to do so we have to structure the project directory in a way that simplyfies the job to the script. My directory Structure is the following one:
```bash
project-root/
├── apache/
│ ├── apache-conf/
│ │ ├── 000-default.conf
│ │ └── ports.conf
│ └── Dockerfile
├── reloader/
│ ├── Makefile
│ ├── main.cpp
│ └── include/
├── docker-compose.yml
├── start.sh
└── shutdown.sh
```

<br><br>
---
<br><br>




## 💻 Running Application

1. Clone the repo
```bash
git clone https://github.com/andrea-ceron/Live-Server_SPA.git
```

2. **Build the Image**.  
```bash
docker compose build
```

3. **Start the Utility**:  
```bash
sh startup.sh ProjectName
```

4. **Close Utility**:
```bash
sh shutdown.sh 
```



<br><br>
---
<br><br>

## 📜 License

Distributed under the **MIT License**.  
See the [`LICENSE`](./LICENSE) file for details.

