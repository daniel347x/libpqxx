/** Definition of the pqxx::basic_connection class template.
 *
 * Instantiations of basic_connection bring connections and policies together.
 *
 * DO NOT INCLUDE THIS FILE DIRECTLY; include pqxx/basic_connection instead.
 *
 * Copyright (c) 2006-2018, Jeroen T. Vermeulen.
 *
 * See COPYING for copyright license.  If you did not receive a file called
 * COPYING with this source code, please notify the distributor of this mistake,
 * or contact the author.
 */
#ifndef PQXX_H_BASIC_CONNECTION
#define PQXX_H_BASIC_CONNECTION

#include "pqxx/compiler-public.hxx"
#include "pqxx/compiler-internal-pre.hxx"

#include <cstddef>
#include <memory>
#include <string>

#include "pqxx/connection_base.hxx"


namespace pqxx
{

/// Base-class template for all libpqxx connection types.
/** Combines connection_base (the highly complex class implementing essentially
 * all connection-related functionality) with a "connection policy."  It's a
 * simpler helper class determining the rules that govern the process of
 * setting up the underlying connection to the backend.
 *
 * The pattern used to combine these classes is the same as for
 * basic_transaction.  The class serves as a base class for various concrete
 * connection classes.  The base class has no virtual functions; those are all
 * in the "policy class" which defines how connections should be established
 * and maintained.  The policy class does have virtual functions.
 */
template<typename CONNECTPOLICY> class basic_connection :
  private CONNECTPOLICY, public connection_base
{
public:
  basic_connection() :
    CONNECTPOLICY(),
    connection_base(*static_cast<CONNECTPOLICY *>(this))
	{ init(); }

  /// The parsing of options is the same as libpq's PQconnect.
  /// See: https://www.postgresql.org/docs/10/static/libpq-connect.html
  explicit basic_connection(const std::string &opt) :
    CONNECTPOLICY(opt),
    connection_base(*static_cast<CONNECTPOLICY *>(this))
	{init();}

  /// See: @c basic_connection(const std::string &opt)
  explicit basic_connection(const char opt[]) :
    basic_connection(opt ? std::string(opt) : std::string()) {}

  explicit basic_connection(std::nullptr_t) : basic_connection() {}

  virtual ~basic_connection() noexcept
	{ close(); }

  using CONNECTPOLICY::options;
};

} // namespace

#include "pqxx/compiler-internal-post.hxx"

#endif
