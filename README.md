# DISAC

DISAC is a blockchain-inspired data integrity system for academic credentials, written entirely in C++ from scratch.

---

## Deprecated — Development Note

This project was originally built as a college assignment. The motivation was simple: it looked difficult, and difficult projects tend to earn extra credit and respect.

That decision came with consequences.

The technology stack was poorly chosen, the architecture was flawed, and the code quality reflects inexperience. Despite that, this project ended up teaching more about real-world software engineering than formal coursework ever did — mostly by forcing mistakes to surface clearly and painfully.

As a learning artifact, it has value. As production-ready software, it does not.

---

## What DISAC Does

DISAC addresses the problem of **verifying the integrity of academic certificates**.

- Students can log in and view their academic certificates
- Administrators can log in and issue new certificates
- Certificates are stored in a local, blockchain-like structure
- Internally, this “blockchain” is implemented as a linked list, where each record is cryptographically linked to the previous one

The goal is to make certificate tampering evident rather than impossible.

---

## Long-Term Ideas (Unimplemented)

- Multi-node architecture
- Distributed verification instead of a single local chain

---

## Usage

### Frontend
- Served at `localhost:5500`
- No frontend framework
- Separate static HTML files
- Login credentials are hardcoded into the frontend (yes, really)

### CLI Tool
- Location: `/core/api/main.cpp`
- Purpose: direct database interaction
- Run with:

./main


### Backend Server
- Built using the Crow C++ framework
- Location: `/core/api/crowMain.cpp`
- Run with:

./crowMain


---

## Status

Deprecated.  
Kept for historical and educational reasons.