#include "CTF.h"
#include "CancellationToken.h"

namespace CTF
{
	void CancellationToken::ThrowIfCancellationRequested() const
	{
		if ( IsCancellationRequested() )
		{
			throw OperationCanceledException();
		}
	}

	CancellationTokenSource::CancellationTokenSource()
		: m_State( std::make_shared<Internal::CancellationState>() )
	{
	}

	void CancellationTokenSource::Cancel() noexcept
	{
		m_State->IsCancellationRequested.store(
			true,
			std::memory_order_release );
	}

	[[nodiscard]]
	bool CancellationTokenSource::IsCancellationRequested() const noexcept
	{
		return m_State->IsCancellationRequested.load(
			std::memory_order_acquire );
	}
}