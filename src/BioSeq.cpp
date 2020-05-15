// [[Rcpp::plugins(cpp11)]]
#include <Rcpp.h>
#include <fstream>
#include <algorithm>
#include "BioSeq.h"
using namespace std;

// -----------------------------------------------------------------------------
// GenomeFeatures
// Ryan D. Crawford
// 05/12/2020
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

// This function reads in the fasta file. Returns true if the file
// was able to be read in
bool BioSeq::parseFasta( )
{
  std::string line;            // Current line in the fasta file
  std::string *curContig;      // Pointer to the current contig
  ifstream    ifs;             // File stream for the fasta file
  int         seqIdx = -1; // Index of the contig being parsed

  // If the file stream has failed to open, return false
  ifs.open( faPath.c_str() );
  if ( ifs.fail() ) return false;

  // Read in the fasta file line by line
  while ( getline( ifs, line ) )
  {
    if (line[0] == '>')
    {
      seqIdx ++; // Advance the couter to the current contig
      // Parse the line to get the name of the contig
      seqNames.push_back( getseqName( line ) );

      // Create an empty contig to add sequences to
      seqs.push_back( "" );
      curContig = & seqs[ seqIdx ];

    } else {

      // Add the line to the current contig
      *curContig += line;
    }
  }

  ifs.close();
  maxContig = seqIdx;
  curSeqName = seqNames.begin();
  convertToUper()
  return true;
}

// This function reads in the fasta file. Returns true if the file
// was able to be read in
bool BioSeq::parseFasta( std::vector< std::string > &faSeq )
{
  std::string *curContig;           // Pointer to the current contig
  ifstream    ifs;                  // File stream for the fasta file
  auto        line = faSeq.begin(); // Iterator for the line in the fasta file
  int         seqIdx = -1;      // Index of the contig being parsed

  // If the file stream has failed to open, return false
  ifs.open( faPath.c_str() );
  if ( ifs.fail() ) return false;

  // Read in the fasta file line by line
  while ( line < faSeq.end() )
  {
    // Check if the first position of the string is a new header
    if ( line->at( 0 ) == '>')
    {
      seqIdx ++; // Advance the couter to the current contig

      // Parse the line to get the name of the contig
      seqNames.push_back( getseqName( *line ) );

      // Create an empty contig to add sequences to
      seqs.push_back( "" );
      curContig = & seqs[ seqIdx ];
    } else {

      // Add the line to the current contig
      *curContig += *line;
    }
  }
  ifs.close();
  convertToUper()
  return true;
}

void BioSeq::convertToUper()
{
  // Any down stream steps with this structure will require it to be in
  // upper case. Test if the first character is lowercase and if it is,
  // transform the entire string to uppercase
  if ( std::islower( seqs[0][0] ) )
  {
    transform( seqs.begin(), seqs.end(), seqs.begin(), ::toupper);
  }
}

bool BioSeq::setFasta( std::string faPath )
{
  this->faPath = faPath;
  return parseFasta();
}

std::string BioSeq::getSeqName( std::string &faHeader )
{
  // Erase the fasta header.
  faHeader.erase( 0, 1 );

  // If the fasta header starts with accn, delete it
  int accnPos = faHeader.find( "accn|" );
  if ( accnPos != string::npos ) faHeader.erase( 0, accnPos + 5 );

  // Find the position of the first space.
  int spacePos = faHeader.find( ' ' );

  // If there are no spaces, return the header as is
  if ( spacePos == string::npos ) return faHeader;

  // Erase everything after the space from the header and return
  int len = spacePos - faHeader.length() + 1;
  return faHeader.erase( spacePos, len );
}

// This function returns the whole genome sequence as a vector
std::vector< std::string > BioSeq::getSeqs( )
{
  return seqs;
}

std::vector< std::string > BioSeq::getNumSeqs()
{
  return seqNames;
}

// Returns the count of the seqs for this genome
int BioSeq::getNumContigs() const
{
  return seqNames.size();
}

// Find which contig
bool BioSeq::getSeqIndex( const std::string &seqName, int &seqIdx )
{
  // If this is the current contig, find it. If the contig name
  // is the same. return true and dont update the index.
  if ( seqName != *curSeqName )
  {
    // Find the contig name in the vector
    curSeqName = find( seqNames.begin(), seqNames.end(), seqName );
  }

  // If the name was not found return false
  if ( curSeqName == seqNames.end() ) return false;
  seqIdx = std::distance( seqNames.begin(), curSeqName );
  return true;
}

// Free the memory associated with the whole genome sequence. The contig
// names are retained. This function exists for instances where mimizing memory
// usage is critical and keeping the whole genome sequence in memory
void BioSeq::clearSeqs( )
{
  seqs.clear();
}

bool BioSeq::getSeqAtCoord(
  const int seqIdx, const int startPos, const int endPos, std::string &seq
  )
{
  // Check that this is a contig in this file
  if ( seqIdx > maxContig ) return false;

  // Check that the requested direction is correct
  if ( startPos > endPos ) return false;

  // Update the sequence to return the substring for this contig. The start
  // is decremented by one because it is one indexed
  int len = endPos - startPos + 1;
  seq = seqs[ seqIdx ].substr( startPos - 1, len );
  return true;
}


// -----------------------------------------------------------------------------