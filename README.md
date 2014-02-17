# This is the README

Authors:
	Samuel Rhody
	Nathan Hourt
	Patrick Biernat
	
Default Parameters:
	Set m = 8
	Set n = 32

Implementation notes:
	To determine F_k_i(S_i), use HMAC with SHA-1

	Use a PRNG to generate all S_i

	Use single instance of k'' for pre-encryption. Key k'' has been randomly chosen to be 4.

	k_i = f_k'(L_i)

	Using qca-qt5 from AUR and Qt 5
