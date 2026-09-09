#!/bin/bash
set -x

if [[ ( -z "$CI_AFF3CT_DEPLOY" ) || ( "$CI_AFF3CT_DEPLOY" != "ON" ) ]]
then
	echo "This job is disabled, try to set the CI_AFF3CT_DEPLOY environment variable to 'ON' to enable it."
	exit 1
fi

cmake --version
mkdir build
mv doc/build doc/built
cd build

if [ -z "$DISTRIBS" ]
then
	echo "Please define the 'DISTRIBS' environment variable."
	exit 1
fi


if [ -z "$CXX" ]
then
	echo "Please define the 'CXX' environment variable."
	exit 1
fi

if [ -z "$THREADS" ]
then
	echo "The 'THREADS' environment variable is not set, default value = 1."
	THREADS=1
fi

if [ -z "$NAME" ]
then
	echo "The 'NAME' environment variable is not set, default value = 'build_deploy_upload_ppa'."
	NAME="build_deploy_upload_ppa"
fi

# These flags end up in the generated 'debian/rules' (see UploadPPA.cmake) and
# are the ones actually used by the Launchpad builders. Without them the
# packages are compiled for the bare x86-64 baseline (SSE2), where MIPP has no
# implementation for a number of 8-bit and 32-bit integer operations and aborts
# at run time with "mipp::<op> (SSE2) is undefined!" as soon as a simulation
# starts. SSE4.2 is the level already used for the other published binaries.
if [ -z "$CFLAGS" ]
then
	echo "The 'CFLAGS' environment variable is not set, default value = '-Wall -funroll-loops -m64 -msse4.2 -faligned-new'."
	CFLAGS="-Wall -funroll-loops -m64 -msse4.2 -faligned-new"
fi

if [ -z "$PPA_DRY_RUN" ]
then
	echo "The 'PPA_DRY_RUN' environment variable is not set, default value = 'ON' (safe default, no upload)."
	PPA_DRY_RUN="ON"
fi

DPUT_HOST="ppa:aff3ct/aff3ct"

if [ -z "$LFLAGS" ]
then
	cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=$CXX \
	         -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="$CFLAGS" \
	         $CMAKE_OPT -DCMAKE_INSTALL_PREFIX="$NAME" \
	         -DAFF3CT_UPLOAD_PPA="ON" -DAFF3CT_DPUT_HOST="$DPUT_HOST" \
	         -DAFF3CT_PPA_DISTRIB="$DISTRIBS"
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
else
	cmake .. -G"Unix Makefiles" -DCMAKE_CXX_COMPILER=$CXX \
	         -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="$CFLAGS" \
	         -DCMAKE_EXE_LINKER_FLAGS="$LFLAGS" \
	         $CMAKE_OPT -DCMAKE_INSTALL_PREFIX="$NAME" \
	         -DAFF3CT_UPLOAD_PPA="ON" -DAFF3CT_DPUT_HOST="$DPUT_HOST" \
	         -DAFF3CT_PPA_DISTRIB="$DISTRIBS"
	rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi
fi

make -j $THREADS -k
rc=$?; if [[ $rc != 0 ]]; then exit $rc; fi

if [ "$PPA_DRY_RUN" = "ON" ]
then
	echo "Dry-run mode: Debian source packages built, skipping Launchpad upload."
	exit 0
fi

# Real upload: refuse to push unsigned packages (Launchpad would reject them
# asynchronously by email while CI shows green).
if ! gpg --list-secret-keys 2>/dev/null | grep -q sec
then
	echo "ERROR: no GPG secret key available to sign the packages; aborting real PPA upload."
	exit 1
fi

if command -v apt-get >/dev/null; then
	apt-get update && apt-get install -y python3-paramiko openssh-client || true
fi

mkdir -p /etc/ssh /root/.ssh /github/home/.ssh ~/.ssh
chmod 700 /root/.ssh /github/home/.ssh ~/.ssh 2>/dev/null || true

cat <<EOF | tee -a /etc/ssh/ssh_known_hosts /root/.ssh/known_hosts /github/home/.ssh/known_hosts ~/.ssh/known_hosts > /dev/null
ppa.launchpad.net ssh-rsa AAAAB3NzaC1yc2EAAAABIwAAAQEA0aKz5UTUndYgIGG7dQBV+HaeuEZJ2xPHo2DS2iSKvUL4xNMSAY4UguNW+pX56nAQmZKIZZ8MaEvSj6zMEDiq6HFfn5JcTlM80UwlnyKe8B8p7Nk06PPQLrnmQt5fh0HmEcZx+JU9TZsfCHPnX7MNz4ELfZE6cFsclClrKim3BHUIGq//t93DllB+h4O9LHjEUsQ1Sr63irDLSutkLJD6RXchjROXkNirlcNVHH/jwLWR5RcYilNX7S5bIkK8NlWPjsn/8Ua5O7I9/YoE97PpO6i73DTGLh5H9JN/SITwCKBkgSDWUt61uPK3Y11Gty7o2lWsBjhBUm2Y38CBsoGmBw==
EOF

ssh-keyscan ppa.launchpad.net | tee -a /etc/ssh/ssh_known_hosts /root/.ssh/known_hosts /github/home/.ssh/known_hosts ~/.ssh/known_hosts > /dev/null 2>&1 || true
chmod 644 /etc/ssh/ssh_known_hosts /root/.ssh/known_hosts /github/home/.ssh/known_hosts ~/.ssh/known_hosts 2>/dev/null || true

if [ -n "$LAUNCHPAD_SSH_KEY" ]; then
	echo "$LAUNCHPAD_SSH_KEY" | tr -d '\r' | tee /root/.ssh/id_ed25519 /github/home/.ssh/id_ed25519 ~/.ssh/id_ed25519 /root/.ssh/id_rsa /github/home/.ssh/id_rsa ~/.ssh/id_rsa > /dev/null
	chmod 600 /root/.ssh/id_* /github/home/.ssh/id_* ~/.ssh/id_* 2>/dev/null || true
fi

cat <<EOF | tee /etc/ssh/ssh_config /root/.ssh/config /github/home/.ssh/config ~/.ssh/config > /dev/null
Host ppa.launchpad.net
    StrictHostKeyChecking no
    GlobalKnownHostsFile /etc/ssh/ssh_known_hosts
    UserKnownHostsFile ~/.ssh/known_hosts
    IdentityFile ~/.ssh/id_ed25519
    IdentityFile ~/.ssh/id_rsa
    IdentityFile /root/.ssh/id_ed25519
    IdentityFile /root/.ssh/id_rsa
    IdentityFile /github/home/.ssh/id_ed25519
    IdentityFile /github/home/.ssh/id_rsa
EOF
chmod 600 /root/.ssh/config /github/home/.ssh/config ~/.ssh/config 2>/dev/null || true

cat <<EOF | tee /etc/dput.cf /root/.dput.cf /github/home/.dput.cf ~/.dput.cf > /dev/null
[ppa]
fqdn = ppa.launchpad.net
method = sftp
incoming = ~%(ppa)s/ubuntu/
login = team-aff3ct
allow_unsigned_uploads = 1
EOF

for dist in $(echo $DISTRIBS | tr ';' ' '); do
	echo "Uploading distribution $dist to Launchpad PPA via SFTP..."

	n=0
	until [ "$n" -ge 5 ]
	do
		make dput_$dist && break
		n=$((n+1))
		echo "Upload busy/locked. Retrying in 30 seconds ($n/5)..."
		sleep 30
	done
	if [ "$n" -ge 5 ]; then
		echo "Failed to upload $dist after 5 attempts."
		exit 1
	fi
done
