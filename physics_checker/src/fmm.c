#include <checker.h>
#include <stdlib.h>

/*

*/


t_octant	*fmm_inner(t_octant *universe)
{
	/*t_octant	*newuniverse;
	
	newuniverse = (t_octant*)malloc(sizeof(t_octant) * 1);
	newuniverse->bodies = (t_body*)malloc(sizeof(t_body) * (universe->end + 1));
	newuniverse->bodies = (t_body*)memcpy(newuniverse->bodies, universe->bodies, sizeof(t_body) * (universe->end + 1));
	*/
	//derp we don't need to do this. To explain BH needs to create a new universe because it relies on the particles interacting in their unchanged state and bh moves them. Keeping it here for explainatory purposes.
	//actually now that I think about it having the newuniverse makes multithreading a bit cleaner although for this the only part that really needs to be threaded is the directcompare at the end
	
	//the algorithm should move out like the petals of an eight petaled flower, moving diagonally outwards and away
	//one needed change for the octant generation code is that we need to keep track of which octant of the set is the far corner a.k.a which one moves away from the other octants
	char	i;
	
	i = 0;
	//if we are at root move into our children
	if (universe->parent == 0)
	{
		while (i < 8)
		{
			fmm(universe->children[i]);
		}
		return(universe);
	}
	//if we are not at root but the only far node is root, move to the farthest corner
	if (universe->parent->parent == 0)
	{
		while (i < 8)
		{
			if (universe->children[i].corner == 1)
				return (fmm(universe->children[i]));
			i++;
		}
	}
	//apply the potential of the far nodes to this one. This is basically just calculating the net effect of the force vectors on it.
	//its basically an attribute of the node that will later get applied to the velocities of all the particles in it.
	computepotential_in(universe->parent);
	//if we have children move to the far corner
	if (universe->children[0] != 0)
	{
		while (i < 8)
		{
			if (universe->children[i].corner == 1)
				return (fmm(universe->children[i]));
			i++;
		}
	}
	//if the node is childless do a direct compare and pulse back.
	directcompare(universe);
	return (fmm_outer(universe);
}

t_octant	*fmm_outer(t_octant *universe)
{
		//same as fmm_inner except instead of moving from the inside to the far corners we move from the far corners in.
}

void	applypotential(t_octant *universe)
{
	//move to the leaves of the tree and then apply the nodes potential to the particles inside and do direct compares
}

void	directcompare(t_octant *universe)
{
	//directly compare the particles of all nodes that share roots parents.
}

void	fmm_real()
{
	
}