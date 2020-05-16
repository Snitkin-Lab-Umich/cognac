#include "Genome.h"
#include "GenomeParser.h"

// -----------------------------------------------------------------------------
// GenomeData
// Ryan D. Crawford
// 05/12/2020
// -----------------------------------------------------------------------------
// This class provides a functionality for parsing the data for multiple
// genomes with multitheadding enabled with tbb.
// -----------------------------------------------------------------------------

#ifndef _GENOME_DATA_
#define _GENOME_DATA_
class GenomeData
{
public:

  // Value ctor for inputs of gff files, fasta files, and the corresponding
  // genome names
  GenomeData(const std::vector< std::string > &gffPaths,
    const std::vector< std::string > &faPaths,
    const std::vector< std::string > &genomeIds);

  // Return the anino acid sequences for all of the input genomes
  std::vector< std::string > getAaSeqs();

  // Return the gene ids for all of the input genomes
  std::vector< std::string > getGeneIds();

  // Write the amino acid sequences to an faa file
  void writeFaa( std::string faaPath );

private:

  // This is a vector of genom class objects
  std::vector< Genome > genomeData;

  // Vectors to store the amino acid IDs and corresponding gene ids
  std::vector< std::string > aaSeqs;

  // Vectors to store the amino acid IDs and corresponding gene ids
  std::vector< std::string > geneIds;

  // The number of genes in this dataset
  int numGenes;

  // Parse the genome data using tbb parallel_for
  void parseGenomeData();

  // Count the number of genes in the dataset
  void countNumGenes();

  // Create the concatenated gene sequences with the gene ids
  void concatenateGenneVecs();
};
#endif

// -----------------------------------------------------------------------------
