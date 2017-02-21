# aff3ct completion                                         -*- shell-script -*-

_aff3ct() {
	local cur prev opts codetype simutype
	COMPREPLY=()
	cur="${COMP_WORDS[COMP_CWORD]}"
	prev="${COMP_WORDS[COMP_CWORD-1]}"

	# determine code type
	for ((i = 0 ; i < ${#COMP_WORDS[*]}; ++ i))
	do
		if [[ ${COMP_WORDS[$i]} == "--cde-type" ]]; then
			codetype=${COMP_WORDS[$i+1]}
		fi
	done

	# determine simu type
	simutype="BFER" # BFER simu by default
	for ((i = 0 ; i < ${#COMP_WORDS[*]}; ++ i))
	do
		if [[ ${COMP_WORDS[$i]} == "--sim-type" ]]; then
			simutype=${COMP_WORDS[$i+1]}
		fi
	done

	# add base opts
	opts="--cde-type --sim-type -v --version -h --help"

	# add contents of Launcher.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "EXIT"  || \
	      ${codetype} == "RSC"        && ${simutype} == "EXIT"  || \
	      ${codetype} == "POLAR"      && ${simutype} == "BFER"  || \
	      ${codetype} == "RSC"        && ${simutype} == "BFER"  || \
	      ${codetype} == "TURBO"      && ${simutype} == "BFER"  || \
	      ${codetype} == "REPETITION" && ${simutype} == "BFER"  || \
	      ${codetype} == "RA"         && ${simutype} == "BFER"  || \
	      ${codetype} == "LDPC"       && ${simutype} == "BFER"  || \
	      ${codetype} == "UNCODED"    && ${simutype} == "BFER"  || \
	      ${codetype} == "RSC"        && ${simutype} == "BFERI" || \
	      ${codetype} == "LDPC"       && ${simutype} == "BFERI" || \
	      ${codetype} == "UNCODED"    && ${simutype} == "BFERI" || \
	      ${codetype} == "POLAR"      && ${simutype} == "GEN" ]]
	then
		opts="$opts --sim-snr-min -m --snr-min-max -M --sim-snr-step -s       \
		      --sim-snr-type -E --sim-stop-time --sim-threads -t --sim-domain \
		      --sim-prec -p --sim-inter-lvl --cde-info-bits -K --cde-size -N  \
		      --src-type --src-path --enc-type --enc-path --mod-type --mod-bps\
		      --mod-ups --mod-cpm-ws --mod-cpm-map --mod-cpm-L --mod-cpm-p    \
		      --mod-cpm-k --mod-cpm-std --mod-const-path --dmod-max           \
		      --dmod-no-sig2 --chn-type --chn-path --chn-blk-fad --qnt-type   \
		      --qnt-int --qnt-bits --qnt-range --dec-type --dec-implem        \
		      --term-no --term-freq --sim-seed --sim-mpi-comm --sim-pyber     \
		      --sim-no-colors --mnt-err-tracker-enable                        \
		      --mnt-err-tracker-filename"
	fi

	# add contents of Launcher_BFER.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "BFER" || \
	      ${codetype} == "RSC"        && ${simutype} == "BFER" || \
	      ${codetype} == "TURBO"      && ${simutype} == "BFER" || \
	      ${codetype} == "REPETITION" && ${simutype} == "BFER" || \
	      ${codetype} == "RA"         && ${simutype} == "BFER" || \
	      ${codetype} == "LDPC"       && ${simutype} == "BFER" || \
	      ${codetype} == "UNCODED"    && ${simutype} == "BFER" ]]
	then
		opts="$opts --sim-benchs -b --sim-benchs-no-ldst -B --sim-debug -d   \
		--sim-debug-limit --snr-sim-trace-path --sim-time-report --cde-coset \
		-c --mnt-max-fe -e  --term-type "
	fi

	# add contents of Launcher_EXIT.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "EXIT" || \
	      ${codetype} == "RSC"        && ${simutype} == "EXIT" ]]
	then
		opts="$opts --sim-siga-min -a --sim-siga-max -A --sim-siga-step"
	fi

	# add contents of Launcher_BFERI.cpp
	if [[ ${codetype} == "LDPC"       && ${simutype} == "BFERI" || \
	      ${codetype} == "UNCODED"    && ${simutype} == "BFERI" ]]
	then
		opts="$opts --sim-benchs -b --sim-debug -d --sim-debug-limit           \
		      --snr-sim-trace-path --sim-time-report --cde-coset -c --itl-type \
		      --itl-path --dmod-ite --mnt-max-fe -e --term-type"
	fi

	# add contents of Launcher_GEN_polar.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "GEN" ]]
	then
		opts="$opts --cde-awgn-fb-path --cde-fb-gen-method --dec-snr \
		      --dec-gen-path --sim-pb-path"
	fi

	# add contents of Launcher_BFER_RA.cpp
	if [[ ${codetype} == "RA"         && ${simutype} == "BFER" ]]
	then
		opts="$opts --dec-ite -i --itl-type --itl-path"
	fi

	# add contents of Launcher_BFER_RSC.cpp
	if [[ ${codetype} == "RSC"        && ${simutype} == "BFER" || \
	      ${codetype} == "RSC"        && ${simutype} == "BFERI" ]]
	then
		opts="$opts --enc-no-buff --enc-poly --dec-type -D --dec-implem \
		      --dec-simd --dec-max"
	fi

	# add contents of Launcher_BFER_polar.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "BFER" ]]
	then
		opts="$opts --sim-pb-path --cde-awgn-fb-path --cde-fb-gen-method  \
		      --cde-sigma --crc-type --enc-no-sys --dec-type -D --dec-ite \
		      -i --dec-lists -L --dec-simd"
	fi

	# add contents of Launcher_BFER_repetition.cpp
	if [[ ${codetype} == "REPETITION" && ${simutype} == "BFER" ]]
	then
		opts="$opts --enc-no-buff --dec-type -D --dec-implem"
	fi

	# add contents of Launcher_BFER_turbo.cpp
	if [[ ${codetype} == "TURBO"      && ${simutype} == "BFER" ]]
	then
		opts="$opts --sim-json-path --crc-type --enc-no-buff --enc-type   \
		      --enc-poly --itl-type --itl-path --dec-type -D --dec-implem \
		      --dec-ite -i --dec-sf --dec-simd --dec-max" 
	fi

	# add contents of Launcher_EXIT_RSC.cpp
	if [[ ${codetype} == "RSC"        && ${simutype} == "EXIT" ]]
	then
		opts="$opts --dec-type -D --dec-implem --dec-max"
	fi

	# add contents of Launcher_EXIT_polar.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "EXIT" ]]
	then
		opts="$opts --sim-pb-path --cde-sigma --cde-awgn-fb-path           \
		      --cde-fb-gen-method --dec-type -D --dec-implem  --dec-ite -i \
		      --dec-lists -L"
	fi

	# add contents of Launcher_BFER_LDPC.cpp
	if [[ ${codetype} == "LDPC"       && ${simutype} == "BFER" || \
	      ${codetype} == "LDPC"       && ${simutype} == "BFERI" ]]
	then
		opts="$opts --dec-type -D --dec-implem --dec-ite -i --cde-alist-path \
		--dec-no-synd --dec-off --dec-norm"
	fi

	# add contents of Launcher_BFER_uncoded.cpp
	if [[ ${codetype} == "UNCODED"    && ${simutype} == "BFER" ]]
	then
		opts="$opts "
	fi

	# add contents of Launcher_BFERI_uncoded.cpp
	if [[ ${codetype} == "UNCODED"   && ${simutype} == "BFERI" ]]
	then
		opts="$opts  "
	fi

	if [[ ${cur} == -* || ${COMP_CWORD} -eq 1 ]] || \
	   [[ ${prev} == @(-q|--quiet) ]] ; then
		COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
		return 0
	fi

	case "${prev}" in
		# awaiting random number or strings
		--sim-snr-min | -m | --snr-min-max | -M | --sim-snr-min | -m |        \
		--snr-min-max | -M | --sim-snr-step | -s | --sim-stop-time |          \
		--sim-threads | -t | --sim-inter-lvl | --cde-info-bits | -K |         \
		--cde-size | -N |                                                     \
		--mod-bps | --mod-ups | --mod-cpm-L | --mod-cpm-p | --mod-cpm-k |     \
		--qnt-int | --qnt-bits | --qnt-range | --qnt-type |                   \
		--sim-benchs | -b | --sim-debug-limit |                               \
		--mnt-max-fe | -e |                                                   \
		--sim-siga-min | -a | --sim-siga-max | -A | --sim-siga-step |         \
		--dmod-ite | --cde-sigma | --dec-snr | --dec-ite |-i | --dec-lists |  \
		-L | --sim-json-path | --dec-off | --dec-norm | --term-freq |         \
		--sim-seed | --sim-mpi-comm | --sim-pyber)
			COMPREPLY=()
			;;

		# awaiting nothing	
		-v | --version | -h | --help | --dmod-no-sig2 | --term-no |        \
		--sim-benchs-no-ldst | -B | --sim-debug | -d | --sim-time-report | \
		--cde-coset | -c | enc-no-buff | --enc-no-sys | --dec-no-synd |    \
		--mnt-err-tracker-enable)
			COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
			;;

		# awaiting something special
		--sim-prec | -p)
			local params="8 16 32 64"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--sim-snr-type | -E)
			local params="EB ES"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--cde-type)
			local params="POLAR TURBO LDPC REPETITION RA RSC UNCODED"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--sim-type)
			local params
			case "${codetype}" in
				POLAR)      params="BFER EXIT"       ;;
				TURBO)      params="BFER"            ;;
				LDPC)       params="BFER BFERI"      ;;
				REPETITION) params="BFER"            ;;
				RA)         params="BFER"            ;;
				RSC)        params="BFER BFERI EXIT" ;;
				UNCODED)    params="BFER BFERI"      ;;
				*)          params="BFER BFERI EXIT" ;;
				
			esac
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--enc-type)
			local params
			case "${codetype}" in
				POLAR)      params="AZCW COSET USER POLAR"     ;;
				RSC)        params="AZCW COSET USER RSC"       ;;
				REPETITION) params="AZCW COSET USER REPETITION";;
				RA)         params="AZCW COSET USER RA"        ;;
				TURBO)      params="AZCW COSET USER TURBO"     ;;
				LDPC)       params="AZCW COSET USER"           ;;
			esac
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dec-type | -D)
			local params
			case "${codetype}" in
				POLAR)      params="SC SCL SCAN"              ;;
				RSC)        params="BCJR BCJR4 LTE CCSDS"     ;;
				REPETITION) params="STD"                      ;;
				RA)         params="STD"                      ;;
				TURBO)      params="LTE CCSDS"                ;;
				LDPC)       params="BP BP_FLOODING BP_LAYERED";;
			esac
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dec-implem)
			local params="GENERIC STD FAST VERY_FAST"
			if [ "${codetype}" == 'LDPC' ]; then
				params="ONMS SPA LSPA GALA"
			fi
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dec-simd)
			local params="INTER INTRA"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--cde-awgn-fb-path | --dec-gen-path | --sim-pb-path | --itl-path | \
		--mod-const-path | --src-path | --enc-path | --chn-path |          \
		--cde-alist-path | --sim-trace-path | --mnt-err-tracker-filename) 
			_filedir
			;;
		
		--src-type)
			local params="RAND RAND_FAST AZCW USER"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--sim-domain)
			local params="LR LLR"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--itl-type)
			local params
			case "${simutype}" in
				BFER)      params="LTE CCSDS RANDOM GOLDEN USER NO" ;;
				BFERI)     params="LTE CCSDS RANDOM COLUMNS GOLDEN USER NO" ;;
			esac
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--chn-type)
			local params="NO AWGN AWGN_FAST AWGN_GSL AWGN_MKL RAYLEIGH"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--chn-blk-fad)
			local params="NO FRAME ONETAP"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--crc-type)
			local params="1-0x1 2-0x1 3-0x3 4-ITU 8-DVB-S2 16-CCITT 16-IBM \
			24-LTEA 32-GZIP"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dec-sf)
			local params="NO LTE LTE_VEC ARRAY"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--mod-cpm-std)
			local params="GSM"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--mod-cpm-map)
			local params="NATURAL GRAY"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--mod-cpm-ws)
			local params="GMSK REC RCOS"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--mod-type)
			local params="BPSK BPSK_FAST PSK PAM QAM CPM USER"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dmod-max)
			local params="MAX MAXL MAXS MAXSS"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dec-max)
			local params="MAX MAXL MAXS"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--qnt-type)
			local params="STD STD_FAST TRICKY"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--term-type)
			local params="STD LEGACY"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--cde-fb-gen-method)
			local params="GA TV"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		*)
			if [[ ${cur} == * || ${COMP_CWORD} -eq 1 ]] || \
				[[ ${prev} == @(-q|--quiet) ]] ; then
				COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
				return 0
			else
				_minimal
			fi
		;;
	esac
} &&
complete -F _aff3ct aff3ct
