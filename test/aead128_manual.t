#!/bin/bash

test_description="manually specified tests"

: "${SHARNESS_TEST_SRCDIR:=.}"
. "$SHARNESS_TEST_SRCDIR/sharness.sh"

# positive tests: manual stuff
test_expect_success "t0: driver internal debug test" "../sp800232-rit_driver -T0 | grep ^476b59b0c2e2bb6a7006cb67aab4f9fe41a2c4df28b919bcb017a67b89938bdee23c5ff2d0432fbf"

test_done
