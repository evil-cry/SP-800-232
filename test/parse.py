import sys

count = key = nonce = pt = ad = ct = None

print("""#!/bin/bash

test_description="manually specified tests"

: "${SHARNESS_TEST_SRCDIR:=.}"
. "$SHARNESS_TEST_SRCDIR/sharness.sh"
""")

for l in sys.stdin:
    l = l.strip()
    if not l and count:
        # emit a test
        if not ad: ad = "??"
        if not pt: pt = "??"
        print("test_expect_success \"T1: encrypt (one-shot) from LWC_AEAD_KAT_128_128.txt\" \"../sp800232-rit_driver -T1 -k %s -n %s -p %s -a %s | grep ^%s\"" % (key, nonce, pt, ad, ct))
        print("test_expect_success \"T2: encrypt (stream) from LWC_AEAD_KAT_128_128.txt\" \"../sp800232-rit_driver -T2 -k %s -n %s -p %s -a %s | grep ^%s\"" % (key, nonce, pt, ad, ct))
        if pt == "??": pt = ct[-32:]
        print("test_expect_success \"T3: decrypt (one-shot) from LWC_AEAD_KAT_128_128.txt\" \"../sp800232-rit_driver -T3 -k %s -n %s -c %s -a %s | grep ^%s\"" % (key, nonce, ct, ad, pt))
        print("test_expect_success \"T4: decrypt (stream) from LWC_AEAD_KAT_128_128.txt\" \"../sp800232-rit_driver -T4 -k %s -n %s -c %s -a %s | grep ^%s\"" % (key, nonce, ct, ad, pt))
        count = key = nonce = pt = ad = ct = None
        continue
    try: lhs, rhs = map(lambda x: x.strip(), l.split("="))
    except: continue
    if lhs == "Count": count = rhs
    if lhs == "Key": key = rhs
    elif lhs == "Nonce": nonce = rhs
    elif lhs == "PT": pt = rhs
    elif lhs == "AD": ad = rhs
    elif lhs == "CT": ct = rhs

print("\ntest_done")
