//
//  newsPicCell.swift
//  balikesiriKesfet
//
//  Created by xloop on 18/10/2017.
//  Copyright © 2017 Xloop. All rights reserved.
//

import UIKit

class newsPicCell: UICollectionViewCell {
    
    @IBOutlet weak var detailPic: UIImageView!
    
    override func awakeFromNib() {
        self.layer.borderColor = UIColor.white.cgColor
        self.layer.borderWidth = 3
    }
}
