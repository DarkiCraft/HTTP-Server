
# 🧪 Git Guide for Our OS Project

Hello Pookies! This guide will help you get started with Git and GitHub for our OS project. Follow the steps carefully and don’t hesitate to ask if you’re confused at any point.

---

## ✅ Step 1: Initial Git Setup
This only needs to be done ONCE.

If you haven’t already, install Git:  
🔗 https://git-scm.com/downloads

After installing, open your terminal and set your identity:

```bash
git config --global user.name "<your-github-username>"
git config --global user.email "<your.email@example.com>"
```

---

## ✅ Step 2: Clone the Repo

Clone the project from GitHub to your computer:

```bash
git clone https://github.com/DarkiCraft/os-project.git
cd os-project
```

---

## ✅ Step 3: Create Your Own Branch

Each teammate should make their own branch to work independently:

```bash
git checkout -b "<your-branch-name>"
```

These are the branch names we will use:
- Ali   -> `ali`
- Ammar -> `amm`
- Arham -> `arh`
- Me    -> `abd`

This lets everyone work without breaking each other’s code.

---

## ✅ Step 4: Work on Your Code

Make changes in your code (in VS Code preferably). Then save and commit your work:

```bash
git add .
git commit -m "<comment changes>"
```
The comment is necessary. Try to make it descriptive to what changes you implemented

---

## ✅ Step 5: Push Your Branch to GitHub

```bash
git push origin "<your-branch-name>"
```

---

## ✅ Step 6: Let Me Know!

Once you’ve pushed your branch, I’ll review it and merge it into the main branch.

---

## 📌 Summary of Commands

```bash
# First time setup
git config --global user.name "<your-github-username>"
git config --global user.email "<your.email@example.com>"

# Clone the repo
git clone https://github.com/DarkiCraft/os-project.git
cd os-project

# Create a branch and switch to it
git checkout -b "<your-module-name>"

# Save and upload your changes
git add .
git commit -m "<comment changes>"
git push origin "<your-branch-name>"
```

---

💬 If you mess up or get errors, don’t panic. Just send a screenshot in the group.