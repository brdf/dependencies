//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007-2011, Image Engine Design Inc. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of Image Engine Design nor the names of any
//       other contributors to this software may be used to endorse or
//       promote products derived from this software without specific prior
//       written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#ifndef IE_CORERI_PTCPARTICLEWRITER_H
#define IE_CORERI_PTCPARTICLEWRITER_H

#include "IECore/ParticleWriter.h"
#include "IECoreRI/TypeIds.h"
#include "IECoreRI/PTCParticleIO.h"

namespace IECoreRI
{

/// The PTCParticleWriter class creates files in renderman ptc format.
/// \ingroup ioGroup
class PTCParticleWriter : public IECore::ParticleWriter
{

	public :

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( PTCParticleWriter, PTCParticleWriterTypeId, IECore::ParticleWriter )

		PTCParticleWriter( );
		PTCParticleWriter( IECore::ObjectPtr object, const std::string &fileName );

	private :

		virtual void doWrite( const IECore::CompoundObject *operands );

		static const WriterDescription<PTCParticleWriter> m_writerDescription;

		struct PTCParticleIO::PTCHeader m_header;

};

IE_CORE_DECLAREPTR( PTCParticleWriter );

} // namespace IECoreRI

#endif // IE_CORERI_PTCPARTICLEWRITER_H