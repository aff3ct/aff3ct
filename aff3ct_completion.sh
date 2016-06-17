# bash completion for aff3ct -*- shell-script -*-

_aff3ct() {
	local cur prev opts codetype simutype
	COMPREPLY=()
	cur="${COMP_WORDS[COMP_CWORD]}"
	prev="${COMP_WORDS[COMP_CWORD-1]}"

	# determine code type
	for ((i = 0 ; i < ${#COMP_WORDS[*]}; ++ i))
	do
		if [[ ${COMP_WORDS[$i]} == "--code-type" ]]; then
			codetype=${COMP_WORDS[$i+1]}
		fi
	done

	# determine simu type
	if [[ $COMP_LINE == *"--simu-type EXIT"* ]]; then
		simutype="EXIT"
	else 
		simutype="BFER"
	fi

	# add base opts
	opts="--code-type --simu-type -v --version -h --help"

	# add contents of Launcher.cpp
	if [[ 
	       ${codetype} == "POLAR"      && ${simutype} == "BFER" || \
	       ${codetype} == "RSC"        && ${simutype} == "BFER" || \
	       ${codetype} == "REPETITION" && ${simutype} == "BFER" || \
	       ${codetype} == "RA"         && ${simutype} == "BFER" || \
	       ${codetype} == "TURBO"      && ${simutype} == "BFER" || \
	       ${codetype} == "POLAR"      && ${simutype} == "EXIT" || \
	       ${codetype} == "RSC"        && ${simutype} == "EXIT"    \
	   ]]
	then
		opts="$opts -K -N --snr-min --snr-max --prec --snr-step --disable-display --stop-time --display-freq \
		      --n-threads --code-gen-method --domain --channel-type --dec-algo --dec-implem"
	fi

	# add contents of Launcher_BFER.cpp
	if [[ 
	       ${codetype} == "POLAR"      && ${simutype} == "BFER" || \
	       ${codetype} == "RSC"        && ${simutype} == "BFER" || \
	       ${codetype} == "REPETITION" && ${simutype} == "BFER" || \
	       ${codetype} == "RA"         && ${simutype} == "BFER" || \
	       ${codetype} == "TURBO"      && ${simutype} == "BFER"    \
	   ]]
	then
		opts="$opts --max-fe --benchs --enable-leg-term --enable-dec-thr --enable-debug"
	fi

	# add contents of Launcher_EXIT.cpp
	if [[ 
	       ${codetype} == "POLAR"      && ${simutype} == "EXIT" || \
	       ${codetype} == "RSC"        && ${simutype} == "EXIT"    \
	   ]]
	then
		opts="$opts --sig-a-max --sig-a-min --sig-a-step"
	fi

	# add contents of Launcher_BFER_RA.cpp
	if [[ 
	       ${codetype} == "RA"         && ${simutype} == "BFER"    \
	   ]]
	then
		opts="$opts --max-iter --interleaver"
	fi

	# add contents of Launcher_BFER_RSC.cpp
	if [[ 
	       ${codetype} == "RSC"        && ${simutype} == "BFER"    \
	   ]]
	then
		opts="$opts --disable-buf-enc --dec-simd-strat"
	fi

	# add contents of Launcher_BFER_polar.cpp
	if [[ 
	       ${codetype} == "POLAR"      && ${simutype} == "BFER"    \
	   ]]
	then
		opts="$opts --disable-sys-enc --max-iter --awgn-codes-dir --bin-pb-path --awgn-codes-file --puncturer -L \
		      --code-sigma --fb-gen-method --crc-type --dec-simd-strat"
	fi

	# add contents of Launcher_BFER_repetition.cpp
	if [[ 
           ${codetype} == "REPETITION" && ${simutype} == "BFER"    \
	   ]]
	then
		opts="$opts"
	fi

	# add contents of Launcher_BFER_turbo.cpp
	if [[ 
           ${codetype} == "TURBO"      && ${simutype} == "BFER"    \
	   ]]
	then
		opts="$opts --max-iter --disable-buf-enc --interleaver --crc-type --scaling-factor --dec-simd-strat"
	fi

	# add contents of Launcher_EXIT_RSC.cpp
	if [[ 
           ${codetype} == "RSC"        && ${simutype} == "EXIT"    \
	   ]]
	then
		opts="$opts "
	fi

	# add contents of Launcher_EXIT_polar.cpp
	if [[ 
           ${codetype} == "POLAR"      && ${simutype} == "EXIT"    \
	   ]]
	then
		opts="$opts --code-sigma --max-iter --awgn-codes-dir --bin-pb-path --awgn-codes-file -L --fb-gen-method"
	fi



	if [[ ${cur} == -* || ${COMP_CWORD} -eq 1 ]] || \
	   [[ ${prev} == @(-q|--quiet) ]] ; then
		COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
		return 0
	fi
 
	case "${prev}" in

		-N | -K | --prec | --qn-bits | --qn-bits | --qpoint-pos | --snr-max | --snr-min | --snr-step | --snr-max | \
		--max-fe | -L | --n-threads | --qmin-max | --stop-time | --disable-freq | --max-iter | --sig-a-min |       \
		--sig-a-max | --sig-a-step | --code-sigma )
			COMPREPLY=()
			;;

		--prec)
			local params="8 16 32 64"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--code-type)
			local params="POLAR TURBO REPETITION RA RSC"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--simu-type)
			local params
			case "${codetype}" in
				POLAR)      params="BFER EXIT" ;;
				RSC)        params="BFER EXIT" ;;
				REPETITION) params="BFER"      ;;
				RA)         params="BFER"      ;;
				TURBO)      params="BFER"      ;;
			esac
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dec-algo)
			local params
			case "${codetype}" in
				POLAR)      params="SC SCL SCAN" ;;
				RSC)        params="BCJR"        ;;
				REPETITION) params="STD"         ;;
				RA)         params="STD"         ;;
				TURBO)      params="BCJR"        ;;
			esac
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dec-simd-strat)
			local params="INTER INTRA"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--dec-implem)
			local params="STD NAIVE FAST VERY_FAST"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--awgn-codes-file | --awgn-codes-dir | --bin-pb-path)
			_filedir
			;;
		
		--code-gen-method)
			local params="RAND LCG AZCW"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--domain)
			local params="LR LLR"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--interleaver)
			local params="LTE RANDOM COLUMNS GOLDEN NO"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--channel-type)
			local params="AWGN AWGN_GSL AWGN_MKL"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--crc-type)
			local params="CRC-1-0x1 CRC-2-0x1 CRC-3-0x3 CRC-4-ITU CRC-8-DVB-S2 CRC-16-CCITT CRC-16-IBM \
			              CRC-16-TIB CRC-24-LTEA CRC-32-GZIP"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--puncturer)
			local params="WANGLIU"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--scaling-factor)
			local params="NO LTE LTE_VEC ARRAY"
			COMPREPLY=( $(compgen -W "${params}" -- ${cur}) )
			;;

		--version | -v | --help | --h | --disable-sys-enc | --disable-freq | --disable-display | --enable-debug | \
		--enable-dec-thr | --enable-leg-term)
			COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
		;;

		*)
			_minimal
		;;
	esac
}

complete -F _aff3ct aff3ct
