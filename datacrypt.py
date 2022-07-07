# data encryption thing
import os

data = open("data.dmf", "r+")


def encrypt(crypt_key, data_to_encrypt):
	list_key = list(crypt_key)
	list_data = list(data_to_encrypt)
	encrypted_data = []
	count = 0 
	for i in range(len(list_data)):
		check = ord(list_data[i]) + ord(list_key[count])
		if count == len(list_key)-1:
			count = 0 
		encrypted_data.append(chr(ord(list_data[i]) + ord(list_key[count])))
		count += 1
		os.system('clear')
		print(f"{int(((i+1)/len(list_data))*100)}% COMPLETE")
	key="".join(str(elem) for elem in list_key)
	out="".join(str(elem) for elem in encrypted_data)
	return out

def decrypt(crypt_key, data_to_decrypt):
	list_key = list(crypt_key)
	list_data = list(data_to_decrypt)
	decrypted_data = []
	count = 0 
	for i in range(len(list_data)):
		if count == len(list_key)-1:
			count = 0 
		decrypted_data.append(chr(ord(list_data[i]) - ord(list_key[count])))
		count += 1
		os.system('clear')
		print(f"{int(((i+1)/len(list_data))*100)}% COMPLETE")
	out="".join(str(elem) for elem in decrypted_data)
	return out
key = input("Password: ")
new = encrypt(key, data.read())

print(decrypt(key, new))
print(new)