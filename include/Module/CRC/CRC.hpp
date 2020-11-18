/*!
 * \file
 * \brief Class module::CRC.
 */
#ifndef CRC_HPP_
#define CRC_HPP_

#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>

#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace crc
	{
		enum class tsk : size_t { build, extract, check, check_packed, SIZE };

		namespace sck
		{
			enum class build        : size_t { U_K1, U_K2, status };
			enum class extract      : size_t { V_K1, V_K2, status };
			enum class check        : size_t { V_K       , status };
			enum class check_packed : size_t { V_K       , status };
		}
	}

/*!
 * \class CRC
 *
 * \brief Adds/builds and checks a Cyclic Redundancy Check (CRC) for a set of information bits.
 *
 * \tparam B: type of the bits in the CRC.
 *
 * Please use CRC for inheritance (instead of CRC).
 */
template <typename B = int>
class CRC : public Module
{
public:
	inline Task&   operator[](const crc::tsk          t);
	inline Socket& operator[](const crc::sck::build   s);
	inline Socket& operator[](const crc::sck::extract s);
	inline Socket& operator[](const crc::sck::check   s);

protected:
	const int K; /*!< Number of information bits (the CRC bits are not included in K) */
	const int size;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K: number of information bits (the CRC bits are included in K).
	 */
	CRC(const int K, const int size);

	/*!
	 * \brief Destructor.
	 */
	virtual ~CRC() = default;

	virtual CRC<B>* clone() const;

	int get_K() const;

	/*!
	 * \brief Gets the size of the CRC (the number of bits for the CRC signature).
	 *
	 * \return the size of the CRC.
	 */
	virtual int get_size();

	/*!
	 * \brief Computes and adds the CRC in the vector of information bits (the CRC bits are often put at the end of the
	 *        vector).
	 */
	template <class A = std::allocator<B>>
	void build(const std::vector<B,A>& U_K1, std::vector<B,A>& U_K2, const int frame_id = -1,
	           const bool managed_memory = true);

	void build(const B *U_K1, B *U_K2, const int frame_id = -1, const bool managed_memory = true);

	template <class A = std::allocator<B>>
	void extract(const std::vector<B,A>& V_K1, std::vector<B,A>& V_K2, const int frame_id = -1,
	             const bool managed_memory = true);

	void extract(const B *V_K1, B *V_K2, const int frame_id = -1, const bool managed_memory = true);

	/*!
	 * \brief Checks if the CRC is verified or not.
	 *
	 * \param V_K:      a vector containing information bits plus the CRC bits.
	 * \param n_frames: you should not use this parameter unless you know what you are doing, this parameter
	 *                  redefine the number of frames to check specifically in this method.
	 *
	 * \return true if the CRC is verified, false otherwise.
	 */
	template <class A = std::allocator<B>>
	bool check(const std::vector<B,A>& V_K, const int frame_id = -1, const bool managed_memory = true);

	// virtual bool check(const B *V_K, const int n_frames = -1, const int frame_id = -1);
	bool check(const B *V_K, const int frame_id = -1, const bool managed_memory = true);

	/*!
	 * \brief Checks if the CRC is verified or not (works on packed bits).
	 *
	 * \param V_K:      a vector of packed bits containing information bits plus the CRC bits.
	 * \param n_frames: you should not use this parameter unless you know what you are doing, this parameter
	 *                  redefine the number of frames to check specifically in this method.
	 *
	 * \return true if the CRC is verified, false otherwise.
	 */
	template <class A = std::allocator<B>>
	bool check_packed(const std::vector<B,A>& V_K, const int frame_id = -1, const bool managed_memory = true);

	// bool check_packed(const B *V_K, const int n_frames = -1, const int frame_id = -1);
	bool check_packed(const B *V_K, const int frame_id = -1, const bool managed_memory = true);

protected:
	virtual void _build(const B *U_K1, B *U_K2, const size_t frame_id);

	virtual void _extract(const B *V_K1, B *V_K2, const size_t frame_id);

	virtual bool _check(const B *V_K, const size_t frame_id);

	virtual bool _check_packed(const B *V_K, const size_t frame_id);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/CRC/CRC.hxx"
#endif

#endif
