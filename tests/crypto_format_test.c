#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>

#include "cryptfs.h"
#include "crypto.h"
#include "format.h"
#include "hash.h"
#include "xalloc.h"

Test(generate_keys, generate_keys, .init = cr_redirect_stdout)
{
    unsigned char *aes_key = xcalloc(1, AES_KEY_SIZE_BYTES);
    RSA *rsa_keypair = RSA_new();

    unsigned char *aes_hash_before = sha256_data(aes_key, AES_KEY_SIZE_BYTES);
    unsigned char *aes_hash_after = NULL;

    generate_keys(aes_key, rsa_keypair);

    // Get RSA pem
    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(bio, rsa_keypair, NULL, NULL, 0, NULL, NULL);
    char *rsa_pem = xcalloc(RSA_size(rsa_keypair) + 1, 1);
    BIO_read(bio, rsa_pem, RSA_size(rsa_keypair));
    BIO_free_all(bio);

    aes_hash_after = sha256_data(aes_key, AES_KEY_SIZE_BYTES);
    cr_assert_arr_neq(aes_hash_before, aes_hash_after, SHA256_DIGEST_LENGTH);

    // Check if PEM starts with "-----BEGIN RSA PRIVATE KEY-----"
    cr_assert_arr_eq(rsa_pem, "-----BEGIN RSA PRIVATE KEY-----",
                     strlen("-----BEGIN RSA PRIVATE KEY-----"));

    free(aes_hash_before);
    free(aes_hash_after);
    free(aes_key);
    RSA_free(rsa_keypair);
    free(rsa_pem);
}

Test(store_keys_in_headers, store_keys_in_headers, .init = cr_redirect_stdout)
{
    struct CryptFS_Header *header = xcalloc(1, sizeof(struct CryptFS_Header));

    RSA *rsa_keypair = RSA_new();
    unsigned char *aes_key = xcalloc(1, RSA_KEY_SIZE_BYTES + 1);

    generate_keys(aes_key, rsa_keypair);
    store_keys_in_headers(header, rsa_keypair, aes_key);

    // Check if the RSA modulus and the RSA public exponent are stored in the
    // header
    BIGNUM *e = BN_new();
    BN_set_word(e, header->keys[0].rsa_e);
    BIGNUM *n = BN_bin2bn((const unsigned char *)&header->keys[0].rsa_n,
                          RSA_KEY_SIZE_BYTES, NULL);

    cr_assert(BN_is_word(e, RSA_EXPONENT), "%s != %d\n", BN_bn2hex(e),
              RSA_EXPONENT);
    cr_assert(BN_cmp(n, RSA_get0_n(rsa_keypair)) == 0, "%s != %s\n",
              BN_bn2hex(n), BN_bn2hex(RSA_get0_n(rsa_keypair)));

    // Decrypt stored AES key and compares it to the constant aes_key_const
    unsigned char *aes_key_decrypted = xcalloc(1, RSA_KEY_SIZE_BYTES + 1);
    int decrypted_size = RSA_private_decrypt(
        RSA_KEY_SIZE_BYTES, header->keys[0].aes_key_ciphered, aes_key_decrypted,
        rsa_keypair, RSA_PKCS1_OAEP_PADDING);
    cr_assert(decrypted_size == AES_KEY_SIZE_BYTES,
              "(decrypted_size = %d) != AES_KEY_SIZE_BYTES", decrypted_size);
    cr_assert_arr_eq(aes_key, aes_key_decrypted, AES_KEY_SIZE_BYTES,
                     "%s != %s\n", aes_key_decrypted, aes_key);

    free(aes_key);
    free(aes_key_decrypted);
    free(header);
    RSA_free(rsa_keypair);
    BN_free(e);
    BN_free(n);
}
