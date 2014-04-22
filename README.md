# Proof of Concept Database for an Untrusted Server

This project demonstrates a database in which all the server sees is ciphertexts. The clients
have keys to their data, and can provide the server with sufficient data to perform various
queries on the data and returning results without actually knowing the plaintext of the data
searched.

This database is based on the work presented in this paper:
http://www.cs.berkeley.edu/~dawnsong/papers/se.pdf

Original Authors (up to commit 151cb350225932ff35210ba3e8eca893faff4eee):
 * Samuel Rhody
 * Nathan Hourt
 * Patrick Biernat

All code since commit 151cb350225932ff35210ba3e8eca893faff4eee is the work of:
 * Nathan Hourt
 * Jennifer Karkoska
 * Mike Moore
	
Default Parameters:
	Set m = 8
	Set n = 32

Implementation notes:
	To determine F_k_i(S_i), use HMAC with SHA-1

	Use a PRNG to generate all S_i

	Use single instance of k'' for pre-encryption. Key k'' has been randomly chosen to be 4.

	k_i = f_k'(L_i)

	Using qca-qt5 from AUR and Qt 5
