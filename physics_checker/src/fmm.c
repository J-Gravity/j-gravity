#include <checker.h>
#include <stdlib.h>

/*

*/

t_octant	*fmm(t_octant *universe)
{
	/*t_octant	*newuniverse;
	
	newuniverse = (t_octant*)malloc(sizeof(t_octant) * 1);
	newuniverse->bodies = (t_body*)malloc(sizeof(t_body) * (universe->end + 1));
	newuniverse->bodies = (t_body*)memcpy(newuniverse->bodies, universe->bodies, sizeof(t_body) * (universe->end + 1));
	*/
	//derp we don't need to do this. To explain BH needs to create a new universe because it relies on the particles interacting in their unchanged state and bh moves them. Keeping it here for explainatory purposes.
	//actually now that I think about it having the newuniverse makes multithreading a bit cleaner although for this the only part that really needs to be threaded is the directcompare at the end
	
	//the algorithm should move out like the petals of an eight petaleed flower, moving diagonally outwards and away
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
			{
				fmm(universe->children[i]);
				break ;
			}
			i++;
		}
		return (universe);
	}
	//apply the potential of the far nodes to this one.
	applypotential(universe, computepotential(universe->parent);
	//if the node is childless do a direct compare with it's neighbors and exit out.
	if (universe->children[0] == 0)
	{
		directcompare(universe);
		return (universe);
	}
	//if we have children move to the far corner
	while (i < 8)
	{
		if (universe->children[i].corner == 1)
		{
			fmm(universe->children[i]);
			break ;
		}
		i++;
	}
	return (universe);
}