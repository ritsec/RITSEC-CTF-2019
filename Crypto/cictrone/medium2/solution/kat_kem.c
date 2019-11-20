#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crypto_kem.h"
#include "b_data.h"
#include "base64.h"

#define KAT_SUCCESS          0
#define KAT_CRYPTO_FAILURE  -4


int main(){
    int            ret_val;
    size_t         outlen;
    unsigned char *ss = 0;
    if (!ss) ss = malloc(crypto_kem_BYTES);
    if (!ss) abort();
    unsigned char *b_sk = base64_decode(b_sk_data, b_sk_len, &outlen);
    unsigned char *b_ct = base64_decode(b_enckey_data, b_enckey_len, &outlen);

    if ( (ret_val = crypto_kem_dec(ss, b_ct, b_sk)) != 0) {
        fprintf(stderr, "crypto_kem_dec returned <%d>\n", ret_val);
        return KAT_CRYPTO_FAILURE;
    }
    printf("The decrypted flag is: %s", base64_encode(ss, crypto_kem_BYTES, &outlen));

    return KAT_SUCCESS;
}
