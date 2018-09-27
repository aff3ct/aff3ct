# aff3ct completion                                         -*- shell-script -*-

_aff3ct() {
	local cur prev opts codetype simutype
	COMPREPLY=()
	cur="${COMP_WORDS[COMP_CWORD]}"
	prev="${COMP_WORDS[COMP_CWORD-1]}"

	# determine code type
	for ((i = 0 ; i < ${#COMP_WORDS[*]}; ++ i))
	do
		if [[ ${COMP_WORDS[$i]} == "--sim-cde-type" || \
		      ${COMP_WORDS[$i]} == "-C" ]]
		then
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
	opts="-C --sim-cde-type --sim-type -v --version -h --help -H --Help"

	# add contents of Launcher.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "EXIT"  || \
	      ${codetype} == "RSC"        && ${simutype} == "EXIT"  || \
	      ${codetype} == "POLAR"      && ${simutype} == "BFER"  || \
	      ${codetype} == "RSC"        && ${simutype} == "BFER"  || \
	      ${codetype} == "BCH"        && ${simutype} == "BFER"  || \
	      ${codetype} == "TURBO"      && ${simutype} == "BFER"  || \
	      ${codetype} == "REP"        && ${simutype} == "BFER"  || \
	      ${codetype} == "RA"         && ${simutype} == "BFER"  || \
	      ${codetype} == "LDPC"       && ${simutype} == "BFER"  || \
	      ${codetype} == "UNCODED"    && ${simutype} == "BFER"  || \
	      ${codetype} == "POLAR"      && ${simutype} == "BFERI" || \
	      ${codetype} == "RSC"        && ${simutype} == "BFERI" || \
	      ${codetype} == "LDPC"       && ${simutype} == "BFERI" || \
	      ${codetype} == "UNCODED"    && ${simutype} == "BFERI" || \
	      ${codetype} == "POLAR"      && ${simutype} == "GEN" ]]
	then
		opts="$opts --sim-snr-min -m --snr-min-max -M --sim-snr-step -s       \
		      --sim-snr-type -E --sim-stop-time --sim-threads -t              \
		      --sim-prec -p --sim-inter-lvl --enc-info-bits -K                \
		      --enc-cw-size -N                                                \
		      --src-type --src-path --enc-type --enc-path --mdm-type --mdm-bps\
		      --mdm-ups --mdm-cpm-ws --mdm-cpm-map --mdm-cpm-L --mdm-cpm-p    \
		      --mdm-cpm-k --mdm-cpm-std --mdm-const-path --mdm-max --mdm-psi  \
		      --mdm-ite --mdm-no-sig2                                         \
		      --chn-type --chn-implem --chn-path --chn-blk-fad --qnt-type     \
		      --qnt-dec --qnt-bits --qnt-range --dec-type --dec-implem        \
		      --ter-no --ter-freq --sim-seed --sim-mpi-comm --sim-pyber       \
		      --sim-no-colors --sim-err-trk --sim-err-trk-rev                 \
		      --sim-err-trk-path --sim-debug-prec --sim-no-legend --except-a2l\
		      --except-no-bt"
	fi

	# add contents of Launcher_BFER.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "BFER" || \
	      ${codetype} == "RSC"        && ${simutype} == "BFER" || \
	      ${codetype} == "BCH"        && ${simutype} == "BFER" || \
	      ${codetype} == "TURBO"      && ${simutype} == "BFER" || \
	      ${codetype} == "REP"        && ${simutype} == "BFER" || \
	      ${codetype} == "RA"         && ${simutype} == "BFER" || \
	      ${codetype} == "LDPC"       && ${simutype} == "BFER" || \
	      ${codetype} == "UNCODED"    && ${simutype} == "BFER" ]]
	then
		opts="$opts --sim-debug -d --sim-debug-fe --sim-debug-limit           \
		      --snr-sim-trace-path --sim-stats --sim-coset -c --mnt-max-fe -e \
		      --ter-type "
	fi

	# add contents of Launcher_EXIT.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "EXIT" || \
	      ${codetype} == "RSC"        && ${simutype} == "EXIT" ]]
	then
		opts="$opts --sim-siga-min -a --sim-siga-max -A --sim-siga-step"
	fi

	# add contents of Launcher_BFERI.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "BFERI" || \
	      ${codetype} == "RSC"        && ${simutype} == "BFERI" || \
	      ${codetype} == "LDPC"       && ${simutype} == "BFERI" || \
	      ${codetype} == "UNCODED"    && ${simutype} == "BFERI" ]]
	then
		opts="$opts --sim-debug -d --sim-debug-fe --sim-debug-limit        \
		      --snr-sim-trace-path --sim-stats --sim-coset -c --itl-type   \
		      --itl-path --itl-cols --itl-uni --sim-ite -I --mnt-max-fe -e \
		      --ter-type"
	fi

	# add contents of Launcher_GEN_polar.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "GEN" ]]
	then
		opts="$opts --enc-fb-awgn-path --enc-fb-gen-method --dec-snr \
		      --dec-gen-path --sim-pb-path"
	fi

	# add contents of Launcher_BFER_RA.cpp
	if [[ ${codetype} == "RA"         && ${simutype} == "BFER" ]]
	then
		opts="$opts --dec-ite -i --itl-type --itl-path --itl-cols --itl-uni"
	fi

	# add contents of Launcher_BFER_BCH.cpp
	if [[ ${codetype} == "BCH"        && ${simutype} == "BFER" ]]
	then
		opts="$opts --dec-corr-pow -T"
	fi

	# add contents of Launcher_BFER_RSC.cpp
	if [[ ${codetype} == "RSC"        && ${simutype} == "BFER" || \
	      ${codetype} == "RSC"        && ${simutype} == "BFERI" ]]
	then
		opts="$opts --enc-no-buff --enc-poly --dec-type -D --dec-implem \
		      --dec-simd --dec-max"
	fi

	# add contents of Launcher_BFER_RSC.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "BFER" || \
	      ${codetype} == "POLAR"      && ${simutype} == "BFERI" ]]
	then
		opts="$opts --sim-pb-path --enc-fb-awgn-path --enc-fb-gen-method \
		      --enc-fb-sigma --dec-type -D --dec-ite -i --dec-implem"
	fi

	# add contents of Launcher_BFER_polar.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "BFER" ]]
	then
		opts="$opts --crc-type --crc-poly --crc-rate --enc-no-sys \
		      --dec-lists -L --dec-simd --dec-polar-nodes         \
		      --dec-partial-adaptive"
	fi

	# add contents of Launcher_BFER_repetition.cpp
	if [[ ${codetype} == "REP"        && ${simutype} == "BFER" ]]
	then
		opts="$opts --enc-no-buff --dec-type -D --dec-implem"
	fi

	# add contents of Launcher_BFER_turbo.cpp
	if [[ ${codetype} == "TURBO"      && ${simutype} == "BFER" ]]
	then
		opts="$opts --enc-json-path --crc-type --crc-poly --crc-rate       \
		      --enc-no-buff --enc-type  --enc-poly --itl-type --itl-path   \
		      --itl-cols --itl-uni --dec-type -D --dec-implem --dec-ite -i \
		      --dec-sf-type --dec-simd --dec-max --dec-sc --dec-fnc        \
		      --dec-fnc-q --dec-fnc-ite-m --dec-fnc-ite-M --dec-fnc-ite-s  \
		      --pct-pattern"
	fi

	# add contents of Launcher_EXIT_RSC.cpp
	if [[ ${codetype} == "RSC"        && ${simutype} == "EXIT" ]]
	then
		opts="$opts --dec-type -D --dec-implem --dec-max"
	fi

	# add contents of Launcher_EXIT_polar.cpp
	if [[ ${codetype} == "POLAR"      && ${simutype} == "EXIT" ]]
	then
		opts="$opts --sim-pb-path --enc-fb-sigma --enc-fb-awgn-path        \
		      --enc-fb-gen-method --dec-type -D --dec-implem  --dec-ite -i \
		      --dec-lists -L"
	fi

	# add contents of Launcher_BFER_LDPC.cpp
	if [[ ${codetype} == "LDPC"       && ${simutype} == "BFER" || \
	      ${codetype} == "LDPC"       && ${simutype} == "BFERI" ]]
	then
		opts="$opts --dec-type -D --dec-implem --dec-ite -i --dec-h-path \
		--dec-no-synd --dec-off --dec-norm --dec-synd-depth --dec-simd"
	fi

	# add contents of Launcher_BFER_uncoded.cpp
	if [[ ${codetype} == "UNCODED"    && ${simutype} == "BFER" ]]
	then
		opts="$opts "
	fi

	# add contents of Launcher_BFERI_uncoded.cpp
	if [[ ${codetype} == "UNCODED"    && ${simutype} == "BFERI" ]]
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
		--sim-snr-min | -m | --snr-min-max | -M | --sim-snr-min | -m |         \
		--snr-min-max | -M | --sim-snr-step | -s | --sim-stop-time |           \
		--sim-threads | -t | --sim-inter-lvl | --enc-info-bits | -K |          \
		--enc-cw-size | -N | --mdm-ite | --chn-gain-occur |                    \
		--mdm-bps | --mdm-ups | --mdm-cpm-L | --mdm-cpm-p | --mdm-cpm-k |      \
		--qnt-dec | --qnt-bits | --qnt-range | --qnt-type |                    \
		--sim-benchs | -b | --sim-debug-limit | --sim-debug-prec |             \
		--mnt-max-fe | -e | -d |                                               \
		--sim-siga-min | -a | --sim-siga-max | -A | --sim-siga-step | -I |     \
		--sim-ite | --enc-fb-sigma | --dec-snr | --dec-ite |-i | --dec-lists | \
		-L | --enc-json-path | --dec-off | --dec-norm | --ter-freq |           \
		--sim-seed | --sim-mpi-comm | --sim-pyber | --dec-polar-nodes |        \
		--itl-cols | --dec-synd-depth | --pct-pattern |                        \
		--dec-fnc-q | --dec-fnc-ite-m | --dec-fnc-ite-M | --dec-fnc-ite-s      )
			COMPREPLY=()
			;;

		# awaiting nothing
		-v | --version | -h | --help |  -H | --Help | --mdm-no-sig2 |      \
		--sim-debug | --sim-debug-fe | --sim-stats | --sim-no-legend |     \
		--sim-coset | -c | enc-no-buff | --enc-no-sys | --dec-no-synd |    \
		--crc-rate | --sim-err-trk | --sim-err-trk-rev | --itl-uni |       \
		--dec-partial-adaptive | --dec-fnc | --dec-sc | --except-a2l |     \
		--except-no-bt | --ter-no                                          )
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

		--sim-cde-type)
			local params="POLAR TURBO LDPC REP RA RSC BCH UNCODED"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--sim-type)
			local params
			case "${codetype}" in
				POLAR)   params="BFER EXIT"       ;;
				TURBO)   params="BFER"            ;;
				LDPC)    params="BFER BFERI"      ;;
				REP)     params="BFER"            ;;
				RA)      params="BFER"            ;;
				BCH)     params="BFER"            ;;
				RSC)     params="BFER BFERI EXIT" ;;
				UNCODED) params="BFER BFERI"      ;;
				*)       params="BFER BFERI EXIT" ;;

			esac
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--enc-type)
			local params
			case "${codetype}" in
				POLAR) params="AZCW COSET USER POLAR"                  ;;
				RSC)   params="AZCW COSET USER RSC"                    ;;
				REP)   params="AZCW COSET USER REP"                    ;;
				RA)    params="AZCW COSET USER RA"                     ;;
				BCH)   params="AZCW COSET USER BCH"                    ;;
				TURBO) params="AZCW COSET USER TURBO"                  ;;
				LDPC)  params="AZCW COSET USER LDPC LDPC_H LDPC_DVBS2" ;;
			esac
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dec-type | -D)
			local params
			case "${codetype}" in
				POLAR) params="SC SCL SCAN"              ;;
				RSC)   params="BCJR BCJR4 LTE CCSDS"     ;;
				REP)   params="STD"                      ;;
				RA)    params="STD"                      ;;
                BCH)   params="ALGEBRAIC"                ;;
				TURBO) params="LTE CCSDS"                ;;
				LDPC)  params="BP BP_FLOODING BP_LAYERED";;
			esac
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dec-implem)
			local params="NAIVE GENERIC STD FAST VERY_FAST"
			if [ "${codetype}" == 'LDPC' ]; then
				params="ONMS SPA LSPA GALA"
			fi
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dec-simd)
			local params="INTER INTRA"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--enc-fb-awgn-path | --dec-gen-path | --sim-pb-path | --itl-path | \
		--mdm-const-path | --src-path | --enc-path | --chn-path |          \
		--dec-h-path | --sim-err-trk-path)
			_filedir
			;;

		--src-type)
			local params="RAND RAND_FAST AZCW USER"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--itl-type)
			local params
			case "${simutype}" in
				BFER)      params="LTE CCSDS RANDOM RAND_COL ROW_COL COL_ROW GOLDEN USER NO" ;;
				BFERI)     params="LTE CCSDS RANDOM RAND_COL ROW_COL COL_ROW GOLDEN USER NO" ;;
			esac
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--chn-type)
			local params="NO AWGN RAYLEIGH RAYLEIGH_USER USER"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--chn-implem)
			local params="STD FAST GSL MKL"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--chn-blk-fad)
			local params="NO FRAME ONETAP"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--crc-type)
			local params="STD FAST INTER"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--crc-poly)
			local params="1-PAR 4-ITU 5-USB 5-EPC 5-ITU 6-ITU 6-DARC 6-CDMA2000-B 6-CDMA2000-A  \
			              7-MMC 7-MVB 8-DVB-S2 8-AUTOSAR 8-CCITT 8-DALLAS 8-DARC 8-SAE-J1850    \
			              8-WCDMA 10-ATM 10-CDMA2000 11-FLEXRAY 12-TELECOM 12-CDMA2000 13-BBC   \
			              14-DARC 15-CAN 15-MPT1327 16-CHAKRAVARTY 16-ARINC 16-CDMA2000         \
			              16-DECT 16-T10-DIF 16-DNP 16-OPENSAFETY-A 16-OPENSAFETY-B 16-PROFIBUS \
			              16-CCITT 16-IBM 17-CAN 21-CAN 24-FLEXRAY 24-RADIX-64 24-LTEA 30-CDMA  \
			              32-KOOPMAN 32-AIXM 32-CASTAGNOLI 32-GZIP"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dec-sf-type)
			local params="LTE LTE_VEC ARRAY"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--mdm-cpm-std)
			local params="GSM"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--mdm-cpm-map)
			local params="NATURAL GRAY"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--mdm-cpm-ws)
			local params="GMSK REC RCOS"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--mdm-type)
			local params="BPSK BPSK_FAST OOK PSK PAM QAM CPM SCMA USER"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--mdm-max)
			local params="MAX MAXL MAXS MAXSS"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--mdm-psi)
			local params="PSI0 PSI1 PSI2 PSI3"
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

		--ter-type)
			local params="STD LEGACY"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--enc-fb-gen-method)
			local params="GA FILE TV"
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
