/*
 * Cereon Template Framework, a C++ 23 standard template library.
 * Copyright (c) 2026 The Aridity Team, all rights reserved.
 *
 * This file is part of the Cereon Template Framework project.
 *
 * Cereon Template Framework is free software: you can redistribute
 * it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or any later version.
 *
 * Cereon Template Framework is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Cereon Template Framework. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CTF_CANCELLATIONTOKEN_H
#define CTF_CANCELLATIONTOKEN_H
#pragma once

#include <CTF.h>
#include <atomic>
#include <memory>
#include <exception>

namespace CTF
{
	/**
	 * @brief Thrown if an operation has been canceled.
	 */
	class OperationCanceledException : public std::exception
	{
	public:
		const char *what() const noexcept override
		{
			return "The operation was canceled.";
		}
	};

	namespace Internal
	{
		struct CancellationState
		{
			std::atomic<bool> IsCancellationRequested = false;
		};
	}

	/**
	 * @brief Propagates notification that operations should be canceled.
	 */
	class CTF_API CancellationToken
	{
	public:
		constexpr CancellationToken() noexcept = default;

		/**
		 * @brief Gets whether this token is capable of being in the canceled state.
		 */
		[[nodiscard]]
		bool CanBeCanceled() const noexcept
		{
			return static_cast<bool>( m_State );
		}

		/**
		 * @brief Gets whether cancellation has been requested for this token.
		 */
		[[nodiscard]]
		bool IsCancellationRequested() const noexcept
		{
			return m_State &&
				m_State->IsCancellationRequested.load(
					std::memory_order_acquire );
		}

		/**
		 * @brief Throws a OperationCanceledException if this token has had cancellation requested.
		 */
		void ThrowIfCancellationRequested() const;

	private:
		friend class CancellationTokenSource;

		explicit CancellationToken(
			std::shared_ptr<Internal::CancellationState> state )
			: m_State( std::move( state ) )
		{
		}

#ifdef _WIN32
#	pragma warning ( push )
#	pragma warning ( disable : 4251 )
#endif // _WIN32
		std::shared_ptr<Internal::CancellationState> m_State;
#ifdef _WIN32
#	pragma warning ( pop )
#endif // _WIN32
	};

	/**
	 * @brief Signals to a CancellationToken that it should be canceled.
	 */
	class CTF_API CancellationTokenSource
	{
	public:
		CancellationTokenSource();

		/**
		 * @brief Gets the CancellationToken associated with this CancellationTokenSource.
		 */
		[[nodiscard]]
		CancellationToken GetToken() const
		{
			return CancellationToken( m_State );
		}

		/**
		 * @brief Communicates a request for cancellation.
		 */
		void Cancel() noexcept;

		/**
		 * @brief Gets whether cancellation has been requested for this CancellationTokenSource.
		 */
		[[nodiscard]]
		bool IsCancellationRequested() const noexcept;

	private:
#ifdef _WIN32
#	pragma warning ( push )
#	pragma warning ( disable : 4251 )
#endif // _WIN32
		std::shared_ptr<Internal::CancellationState> m_State;
#ifdef _WIN32
#	pragma warning ( pop )
#endif // _WIN32	
	};
}

#endif // !CTF_CANCELLATIONTOKEN_H
