ospfs_create(struct node      *dir,
             struct dentry    *dentry,
             int               node,
             struct nameidata *nd)
{
	//1. Check for -FEXISTS error and check if directory is empty
	ospfs_inode_t * dir_oi = ospfs_inode(dir->i_no);
	if (dir_oi->oi_ftype != OSPFS_FTYPE_DIR)
	{
		return -EIO;
	}
	if (dentry->d_name.len > OSPFS_MAXIMUMLEN)
	{
		return -ENAMETOLONG;
	} 
	if (find_dir_entry(dir_oi, dentry->d_name.name, dentry->d_name.len) != NULL)
	{
		return -EEXIST;
	}
	
	//2. Find an empty inode. set 'entry_ino' variable to its node #
	uint32_t entry_ino = find_free_inode(); // helper function that we will make later
	if (entry_ino == 0)
	{
		return -ENOSPC;
	}
	ospfs_inode_t * fil_oi = ospfs_inode(entry_ino);
	if (file_oi == NULL)
	{
		reuturn -EIO;
	}
	// 3. Initialize directory entry & inode
	file_oi->oi_size = 0;
	file_oi->oi_ftype = OSPFS_FTYPE_REG;
	file_oi->oi_nlink = 1;
	file_oi->oi_node = node;
	// Create a free directory entry
	ospfs_direntry_t * new_entry = create_blank_direntry(dir_oi); // Not yet implemented
	if (IS_ERR(new_entry))
	{
		return PTR_ERR(new_entry); // defined in create_blank_direntry
	}
	new_entry->od_ino = entry_ino;
	memcpy(new_entry->od_name, dentry->d_name.name, d_entry->d_name.len);
	new_entry->od_name(dentry->d_name.len] = '\0';
	/* skelton code to instatiate dir entry*/
	
}
static inline uint32_t find_free_inode(void)
{
	uint32_t inode_no;
	ospfs_inode_t *new_inode_loc;
	for (inode_no = 2; inode_no < ospfs_super->os_ninodes; inode_no++)
	{
		new_inode_loc = ospfs_inode(inode_no);	
		if (new_inode_loc->oi_nlink == 0)
		{
			return inode_no;
		}
	}
	return 0;
}

ospfs_directory_t create_blank_directory(ospfs_inode_t *dir_oi)
{		
	ospfs_direntry_t * od;
	if ( dir->oi_ftype != OSPFS_FTYPE_DIR)
	{
		return ERR_PTR(-EIO);
	}
	size_t offset;
	// 1. Checking existing directory data for a non-empty entry. Return one if found
	for (offset = 0; offset < dir_oi->oi_size; offset += OSPFS_DIRENTRY_SIZE)
	{
		od = ospfs_inode_data(dir_oi, offset);
		if (od->od_ino == 0)
		{
			return od;
		}
	}
	// 2. If no empty, add a block to the directory ...
	uint32_t new_size = (ospfs_size2nblocks(dir_oi->oi_size) + 1) * OSPFS_BLKSIZE;
	int rval = change_size(dir_oi, new_size);
	if (rval != 0)
	{
		return ERR_PTR(rval);
	}
	// probably redundant
	dir_oi->oi_size = new_size;
	return ospfs_inode_data(dir_oi, offset);
}

static int ospfs_link(struct dentry *src_dentry, struct inode *dir, struct dentry *dst_entry)
{
	ospfs_inode_t *dir_oi = ospfs_inode(dir->i_no);
	ospfs_inode_t* src_oi = ospfs_inode(src_dentry->d_inode->i_no);
	ospfs_direntry_t *new_entry;
	// same inode number, increment link count
	// Error checkings
	if (dir_oi == NULL || dir_oi->oi_ftype != OSPFS_FTYPE_DIR || src_oi->oi_nlinks + 1 == 0)
	{
		return -EIO;
	}
	if (dst_entry->d_name.len > OSPDS_MAXNAMELEN)
	{
		return -ENAMETOOLONG;
	}
	if (find_direntry(diroi, dst_entry->d_name.name, dstentry->d_name.len) != NULL)
	{
		return -EEXIST;
	}
	new_entry = create_blank_direntry(dir_oi);
	if (IS_ERR(new_entry))
	{
		return PTR_ERR(new_entry);
	}
	else if (new_entry == NULL)
	{
		return -EIO;
	}
	new_entry->od_ino = src_entry->d_inode->i_no;
	memcpy(new_entry->od_name, dst_entry->d_name.name , dst_entry.name.len);
	new_entry->od_name[dst_entry->d_name.len] = '\0';
	src_oi->nlink++;
	return 0;
}

static int ospfs_symlink(struct dentry *src_dentry, struct inode *dir, struct dentry *dst_entry)
{

}

// for conditional symlink
// to check if current process is root. current->uid == 0;
// implemented in ospfs_follow_link and ospfs_symlink
// conidtion stored as ?'TRUE_CASE\0':'FALSE_CASE'
// in for a symlink oi_symlink is name of file