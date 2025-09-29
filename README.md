
# 🌐 Live-Server_SPA

**Live-Server_SPA** is a utility designed to simplify the development of **Single Page Applications (SPA)** by isolating the client-side implementation from the server-side one.  
As a *live server*, it provides **real-time reload** whenever the code is modified.

---

<br/>

## 🎯 Goals

- 🔹 **Isolate frontend code** → avoid building a new server for every SPA project  
- 🔹 **Server-side rendering** of the frontend code  
- 🔹 **Real-time reload** → every code change is instantly reflected in the browser  

---

<br/>

## ⚡ Features

- 🗂️ **Frontend code injection** → when the utility starts, specify the project directory, and the frontend is automatically loaded into the Docker container  
- 🎭 **Server-side rendering** → the internal server handles web app rendering  
- 👀 **File monitoring with inotify** → detects changes in the Linux filesystem  
- 🔄 **Browser reload notifications** → modifications are processed, and the web app is reloaded  

---

<br/>

## 🛠️ Technologies

- 🖧 **Apache** → acts as a proxy for request redirection  
- 💻 **C++** → used to build the server and monitor filesystem changes  
- 📦 **Docker** → runs the entire application  
- 🐚 **Bash scripts** → used for application startup and frontend directory loading  

---

<br/>

## 📋 Requirements

Before running the app, make sure you have:

- 🐧 **Linux OS or WSL**  
  > The utility relies on the Linux `inotify` syscall, so both the utility **and your IDE** must run on Linux (or WSL on Windows).  
- 🐳 **Docker** (with or without Docker Desktop)  

---

<br/>

## 💻 Running on Windows

1. Install **WSL** (e.g., Ubuntu).  
2. **Copy your project folder into WSL**.  
3. Open **VS Code with the WSL extension**:  
   - Install the **Remote - WSL** extension  
   - Click the **colored rectangle in the bottom-left corner**  

   ![Colored rectangle in the bottom-left corner](docs/image_1.png)

   *Connect to WSL*  

   ![Connect to WSL](docs/image_2.png)

   *VS Code running inside WSL*  

   ![VS Code running on WSL](docs/image_3.png)

4. Once connected, you are fully working inside your Linux subsystem.  
   > Every code change will now be detected in real-time and reloaded by the application 🚀  

5. To start the utility, `startup.sh` is designed to automatically locate your project.  
   You just need to run:

```bash
sh startup.sh <project-name>
```
Since we are not specifying the relative path to the project the script tries to find the project. In order to do so we have to structure the project directory in a way that simplyfies the job to the script. The suggested directory structure is the following one:
```bash
├── c++/
│ ├── project A
│ └── project B
│ └── project C
├── js/
│ ├── project D
│ ├── project E
├── utilities/
│ ├── Live Server
```
So if we want to work on project D and launch our Live Server:
- open the terminal on Live server directory;
- launch the app following the instruction specified in the next chapter;
- specify the name of the project we want to run.  

```bash
sh startup.sh ProjectName
```

with this command the bash script will cd onto the main directory that includes all the techologies and find the path of the project. Once it has found it, it will automatically look for the subdirectory ./client and load that subdirectory.

> If you do not want to implement that directory structure you can alter the startup.sh file and increase or decrease the value of *maxdepth*. That will increase or decrease the recursion depth of the Linux find command. Be careful the greater the recursion depth the more files the find command will process.

```bash
risultato="$(find . -maxdepth 3 -type d -iname ${PROJECT_PATH} -print -quit)/client"
```
---
<br/>


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

---
<br/>

## 📜 License

Distributed under the **MIT License**.  
See the [`LICENSE`](./LICENSE) file for details.

