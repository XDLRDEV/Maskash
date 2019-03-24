/*
	This file is part of cpp-ethereum.

	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <libethcore/Common.h>
#include <libdevcrypto/Common.h>
#include <libdevcore/RLP.h>
#include <libdevcore/SHA3.h>

#include "../libsnark/donator2/interface.hpp"

#include <boost/optional.hpp>

namespace dev
{
namespace eth
{

struct EVMSchedule;

/// Named-boolean type to encode whether a signature be included in the serialisation process.
enum IncludeSignature
{
	WithoutSignature = 0,	///< Do not include a signature.
	WithSignature = 1,		///< Do include a signature.
};

enum class CheckTransaction
{
	None,
	Cheap,
	Everything
};

/// Encodes a transaction, ready to be exported to or freshly imported from RLP.
class TransactionBase
{
public:
	/// Constructs a null transaction.
	TransactionBase() {}

	/// Constructs a transaction from a transaction skeleton & optional secret.
	/// Maskash marsCatXdu web3的 signTransaction 最终调用的构造器
	TransactionBase(TransactionSkeleton const& _ts, Secret const& _s = Secret());

	/// Constructs a signed message-call transaction.
	TransactionBase(u256 const& _value, u256 const& _gasPrice, u256 const& _gas, Address const& _dest, bytes const& _data, u256 const& _nonce, Secret const& _secret): m_type(MessageCall), m_nonce(_nonce), m_value(_value), m_receiveAddress(_dest), m_gasPrice(_gasPrice), m_gas(_gas), m_data(_data) { sign(_secret); }

	/// Constructs a signed contract-creation transaction.
	TransactionBase(u256 const& _value, u256 const& _gasPrice, u256 const& _gas, bytes const& _data, u256 const& _nonce, Secret const& _secret): m_type(ContractCreation), m_nonce(_nonce), m_value(_value), m_gasPrice(_gasPrice), m_gas(_gas), m_data(_data) { sign(_secret); }

	/// Constructs an unsigned message-call transaction.
	TransactionBase(u256 const& _value, u256 const& _gasPrice, u256 const& _gas, Address const& _dest, bytes const& _data, u256 const& _nonce = 0): m_type(MessageCall), m_nonce(_nonce), m_value(_value), m_receiveAddress(_dest), m_gasPrice(_gasPrice), m_gas(_gas), m_data(_data) {}

	/// Constructs an unsigned contract-creation transaction.
	TransactionBase(u256 const& _value, u256 const& _gasPrice, u256 const& _gas, bytes const& _data, u256 const& _nonce = 0): m_type(ContractCreation), m_nonce(_nonce), m_value(_value), m_gasPrice(_gasPrice), m_gas(_gas), m_data(_data) {}

	/// Constructs a transaction from the given RLP.
	explicit TransactionBase(bytesConstRef _rlp, CheckTransaction _checkSig);

	/// Constructs a transaction from the given RLP.
	explicit TransactionBase(bytes const& _rlp, CheckTransaction _checkSig): TransactionBase(&_rlp, _checkSig) {}

	/// Checks equality of transactions.
	bool operator==(TransactionBase const& _c) const { return m_type == _c.m_type && (m_type == ContractCreation || m_receiveAddress == _c.m_receiveAddress) && m_value == _c.m_value && m_data == _c.m_data; }
	/// Checks inequality of transactions.
	bool operator!=(TransactionBase const& _c) const { return !operator==(_c); }

	/// @returns sender of the transaction from the signature (and hash).
	/// @throws TransactionIsUnsigned if signature was not initialized
	Address const& sender() const;
	/// Like sender() but will never throw. @returns a null Address if the signature is invalid.
	Address const& safeSender() const noexcept;
	/// Force the sender to a particular value. This will result in an invalid transaction RLP.
	void forceSender(Address const& _a) { m_sender = _a; }

	/// @throws TransactionIsUnsigned if signature was not initialized
	/// @throws InvalidSValue if the signature has an invalid S value.
	void checkLowS() const;

	/// @throws InvalidSValue if the chain id is neither -4 nor equal to @a chainId
	/// Note that "-4" is the chain ID of the pre-155 rules, which should also be considered valid
	/// after EIP155
	void checkChainId(int chainId = -4) const;

	/// @returns true if transaction is non-null.
	explicit operator bool() const { return m_type != NullTransaction; }

	/// @returns true if transaction is contract-creation.
	bool isCreation() const { return m_type == ContractCreation; }

	/// Serialises this transaction to an RLPStream.
	/// @throws TransactionIsUnsigned if including signature was requested but it was not initialized
	void streamRLP(RLPStream& _s, IncludeSignature _sig = WithSignature, bool _forEip155hash = false) const;

	/// @returns the RLP serialisation of this transaction.
	bytes rlp(IncludeSignature _sig = WithSignature) const { RLPStream s; streamRLP(s, _sig); return s.out(); }

	/// @returns the SHA3 hash of the RLP serialisation of this transaction.
	h256 sha3(IncludeSignature _sig = WithSignature) const;

	/// @returns the amount of ETH to be transferred by this (message-call) transaction, in Wei. Synonym for endowment().
	u256 value() const { return m_value; }

	/// @returns the base fee and thus the implied exchange rate of ETH to GAS.
	u256 gasPrice() const { return m_gasPrice; }

	/// @returns the total gas to convert, paid for from sender's account. Any unused gas gets refunded once the contract is ended.
	u256 gas() const { return m_gas; }

	/// @returns the receiving address of the message-call transaction (undefined for contract-creation transactions).
	Address receiveAddress() const { return m_receiveAddress; }

	/// Synonym for receiveAddress().
	Address to() const { return m_receiveAddress; }

	/// Synonym for safeSender().
	Address from() const { return safeSender(); }

	/// @returns the data associated with this (message-call) transaction. Synonym for initCode().
	bytes const& data() const { return m_data; }

	/// @returns the transaction-count of the sender.
	u256 nonce() const { return m_nonce; }



	/// Maskash marsCatXdu 新增字符串字段
	std::string maskashMsg() const { return m_maskashMsg; }

	/// 构造：购币者发送给铸币者的 铸币请求 的核心信息
	std::string makeMintReqString(uint256 Usk, uint256 p, uint256 v) { return mskTxTmp;	}

	/// 构造：铸币者发送到网络的 铸币交易 的核心信息
	std::string makeMintReqString(uint256 kmint, uint256 v, uint256 upk) { return mskTxTmp; }

	/// 构造：零币转账发起者发送到网络的核心信息
	std::string makeTransferZero(uint256 Rpk, uint256 pr, uint256 vr, uint256 Ssk, uint256 ps, uint256 vs) { return mskTxTmp; }	

	/// 构造：整币转账发起者发送到网络的核心信息
	std::string makeTransferOne(uint256 Rpk, uint256 pr, uint256 vr, uint256 Ssk) { return mskTxTmp; }



	/// Sets the nonce to the given value. Clears any signature.
	void setNonce(u256 const& _n) { clearSignature(); m_nonce = _n; }

	/// @returns true if the transaction was signed
	bool hasSignature() const { return m_vrs.is_initialized(); }

	/// @returns true if the transaction was signed with zero signature
	bool hasZeroSignature() const { return m_vrs && isZeroSignature(m_vrs->r, m_vrs->s); }

	/// @returns true if the transaction uses EIP155 replay protection
	bool isReplayProtected() const { return m_chainId != -4; }

	/// @returns the signature of the transaction (the signature has the sender encoded in it)
	/// @throws TransactionIsUnsigned if signature was not initialized
	SignatureStruct const& signature() const;

	void sign(Secret const& _priv);			///< Sign the transaction.

	/// @returns amount of gas required for the basic payment.
	int64_t baseGasRequired(EVMSchedule const& _es) const { return baseGasRequired(isCreation(), &m_data, _es); }

	/// Get the fee associated for a transaction with the given data.
	static int64_t baseGasRequired(bool _contractCreation, bytesConstRef _data, EVMSchedule const& _es);

protected:
	/// Type of transaction.
	enum Type
	{
		NullTransaction,				///< Null transaction.
		ContractCreation,				///< Transaction to create contracts - receiveAddress() is ignored.
		MessageCall						///< Transaction to invoke a message call - receiveAddress() is used.
	};

	static bool isZeroSignature(u256 const& _r, u256 const& _s) { return !_r && !_s; }

	/// Clears the signature.
	void clearSignature() { m_vrs = SignatureStruct(); }

	Type m_type = NullTransaction;		///< Is this a contract-creation transaction or a message-call transaction?
	u256 m_nonce;						///< The transaction-count of the sender.
	u256 m_value;						///< The amount of ETH to be transferred by this transaction. Called 'endowment' for contract-creation transactions.
	Address m_receiveAddress;			///< The receiving address of the transaction.
	u256 m_gasPrice;					///< The base fee and thus the implied exchange rate of ETH to GAS.
	u256 m_gas;							///< The total gas to convert, paid for from sender's account. Any unused gas gets refunded once the contract is ended.
	bytes m_data;						///< The data associated with the transaction, or the initialiser if it's a creation transaction.

	std::string mskTxTmp;				// Maskash marsCatXdu Maskash交易信息的临时字符串
	std::string m_maskashMsg;			// Maskash marsCatXdu 新增字符串字段

	boost::optional<SignatureStruct> m_vrs;	///< The signature of the transaction. Encodes the sender.
	int m_chainId = -4;					///< EIP155 value for calculating transaction hash https://github.com/ethereum/EIPs/issues/155

	mutable h256 m_hashWith;			///< Cached hash of transaction with signature.
	mutable Address m_sender;			///< Cached sender, determined from signature.
};

/// Nice name for vector of Transaction.
using TransactionBases = std::vector<TransactionBase>;


// marsCatXdu marked   这个输出到哪里了？
/// Simple human-readable stream-shift operator.
inline std::ostream& operator<<(std::ostream& _out, TransactionBase const& _t)
{
	_out << _t.sha3().abridged() << "{";
	if (_t.receiveAddress())
		_out << _t.receiveAddress().abridged();
	else
		_out << "[CREATE]";

	_out << "/" << _t.data().size() << "$" << _t.value() << "+" << _t.gas() << "@" << _t.gasPrice();
	_out << "<-" << _t.safeSender().abridged() << " #" << _t.nonce() << "}";
	return _out;
}

}
}
