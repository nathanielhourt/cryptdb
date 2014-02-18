from Crypto.Cipher import AES
from Crypto.Random import random
from Crypto.Util import Counter


'''
' This function is the pseudorandom generator G, uses AES CTR mode
'''
def PRG_G(input, key, ctr):
	# Use AES with Counter mode
	cipher = AES.new(key, AES.MODE_CTR, counter=ctr)
	seed = key
	ret = cipher.encrypt(seed) + cipher.encrypt(seed)
	return ret

def pad_to_16(input):
	# Convert input to bytearray for padding
	data = input
	length = 16 - (len(input) % 16)
	data += bytes([length]) * length
	# Must convert output to bytes for immutability
	return bytes(data)	

# Use a CBC-MAC of the input for a pseudorandom function
def PRF(input, key):
	# Use AES CBC-MAC
	cipher = AES.new(key, AES.MODE_CBC, '\x00'*AES.block_size)
	return cipher.encrypt(pad_to_16(bytearray(input)))

# Pads not a word to the word size
def makeword(not_word, word_size):
	nw_bytes = bytearray(input, 'utf-8')
	length = word_size - (len(not_word) % word_size)
	nw_bytes += bytes([length]) * length
	return bytes(data)

def read_in_input(file):
	pass

'''
'	This uses the same key for all words in the database 
'''
def main():
	n = 16
	m = n - 1

	#word_list, num_words = read_in_input()
	#First, assume all words are of the same length
	word_list = ['Nnnoo', 'words', 'hearr']
	num_words = 3
	word_key = b'Sixteen byte key'
	def generate_S_values():
		size = n - m
		ret = []
		for i in range(num_words):
			ret.append(random.getrandbits(size))
		return ret

	S = generate_S_values();
	
	# Create T_i
	def generate_T_values(S):
		ret = []
		for s in S:
			tmp = bytearray([s])
			tmp += PRF(tmp, word_key)
			ret.append(tmp)
		return ret

	T = generate_T_values(S)

	ctxts = []
	def sxor(word, t):
		ret = bytearray(b'')
		for i in range(word.size):
			ret += (word[i] ^ t[i])
	for i in range(num_words):
		ctxt = sxor(bytearray(word_list[i], 'utf-8'), T[i])
		print(ctxt)
	# AES example

	# IV would most likely be constant
	ctr = Counter.new(128)
if __name__ == "__main__":
	main()